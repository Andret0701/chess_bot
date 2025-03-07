#include "min_max.h"
#include "heuristic/heuristic.h"
#include "game_history.h"
#include "../utils/utils.h"
#include "../engine/piece_moves.h"
#include <stdio.h>
#include "move_sort.h"
#include "transposition_table.h"

SearchResult min_max(Color bot_color, BoardState *board_state, BoardStack *stack, uint8_t max_depth, uint8_t depth, BoardScore alpha, BoardScore beta, clock_t start, double seconds)
{
    if (has_timed_out(start, seconds))
        return (SearchResult){(BoardScore){0, UNKNOWN, 0}, false};
    push_game_history(board_state->board);
    if (threefold_repetition() || has_50_move_rule_occurred())
    {
        BoardScore score = score_board(board_state, depth, false);
        pop_game_history();
        score.result = DRAW;
        return (SearchResult){score, true};
    }

    // if in check - extend search
    if (board_state->white_check || board_state->black_check)
        max_depth++;

    // --- TT Lookup ---
    uint64_t hash = hash_board(&board_state->board);
    TT_Entry tt_entry;
    if (TT_lookup(hash, &tt_entry, max_depth - depth))
    {
        BoardScore score = (BoardScore){tt_entry.score, tt_entry.result, depth + tt_entry.depth};
        if (tt_entry.type == EXACT)
        {
            pop_game_history();
            return (SearchResult){score, true};
        }
        else if (tt_entry.type == UPPERBOUND)
        {
            beta = max_score(beta, score, BLACK);
            if (is_better_equal(alpha, beta, WHITE))
            {
                pop_game_history();
                return (SearchResult){score, true};
            }
        }
        else if (tt_entry.type == LOWERBOUND)
        {
            alpha = max_score(alpha, score, WHITE);
            if (is_better_equal(beta, alpha, BLACK))
            {
                pop_game_history();
                return (SearchResult){score, true};
            }
        }
    }

    if (depth == max_depth)
    {
        bool finished = is_finished(board_state);
        Result result = get_result(board_state, finished);
        finished |= result != UNKNOWN;

        BoardScore score;
        if (!finished)
        {
            SearchResult q_result = quiesce(board_state, stack, alpha, beta, depth, start, seconds);
            if (!q_result.valid)
            {
                pop_game_history();
                return (SearchResult){(BoardScore){0, UNKNOWN, 0}, false};
            }

            score = (BoardScore){q_result.board_score.score, result, depth};
        }
        else
            score = score_board(board_state, depth, finished);

        // --- TT Store for terminal nodes ---
        TT_store(hash, max_depth - depth, score.score, score.result, EXACT);
        pop_game_history();
        return (SearchResult){score, true};
    }

    uint16_t base = stack->count;
    generate_moves(board_state, stack);

    bool finished = base == stack->count;
    Result result = get_result(board_state, finished);
    finished |= result != UNKNOWN;
    if (finished)
    {
        BoardScore score = score_board(board_state, depth, finished);
        stack->count = base;

        // --- TT Store for no legal moves ---
        TT_store(hash, max_depth - depth, score.score, score.result, EXACT);
        pop_game_history();
        return (SearchResult){score, true};
    }

    sort_moves(board_state, stack, base);

    BoardScore best_score = get_worst_score(board_state->board.side_to_move);
    for (uint16_t i = base; i < stack->count; i++)
    {
        BoardState *next_board_state = &stack->boards[i];
        SearchResult search_result = min_max(bot_color, next_board_state, stack, max_depth, depth + 1, alpha, beta, start, seconds);

        // --- Check if timed out ---
        if (!search_result.valid)
        {
            stack->count = base;
            pop_game_history();
            return (SearchResult){best_score, false};
        }

        // --- Check if has mate in 1 ---
        bool is_mate = has_won(search_result.board_score.result, board_state->board.side_to_move);
        bool is_mate_in_1 = search_result.board_score.depth == depth + 1 && is_mate;
        if (bot_color == board_state->board.side_to_move && is_mate_in_1)
        {
            stack->count = base;
            // Maybe add TT_store if it performs better
            pop_game_history();
            return search_result;
        }
        else if (bot_color != board_state->board.side_to_move && is_mate)
        {
            stack->count = base;
            // Maybe add TT_store if it performs better
            pop_game_history();
            return search_result;
        }

        BoardScore score = search_result.board_score;
        best_score = max_score(best_score, score, board_state->board.side_to_move);
        if (board_state->board.side_to_move == WHITE)
        {
            alpha = max_score(alpha, score, WHITE);
            if (is_better_equal(beta, alpha, BLACK))
            {
                stack->count = base;
                TT_store(hash, max_depth - depth, best_score.score, best_score.result, LOWERBOUND);
                pop_game_history();
                return (SearchResult){best_score, true};
            }
        }
        else
        {
            beta = max_score(beta, score, BLACK);
            if (is_better_equal(alpha, beta, WHITE))
            {
                stack->count = base;
                TT_store(hash, max_depth - depth, best_score.score, best_score.result, UPPERBOUND);
                pop_game_history();
                return (SearchResult){best_score, true};
            }
        }
    }

    stack->count = base;

    // --- TT Store for final value ---
    TT_store(hash, max_depth - depth, best_score.score, best_score.result, EXACT);
    pop_game_history();
    return (SearchResult){best_score, true};
}

void print_search_result(SearchResult search_result)
{
    if (search_result.valid)
        print_score(search_result.board_score);
    else
        printf("Invalid\n");
}