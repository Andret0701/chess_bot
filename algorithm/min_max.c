#include "min_max.h"
#include "heuristic.h"
#include "game_history.h"
#include "../utils.h"
#include "../engine/piece_moves.h"
#include <stdio.h>
#include "move_sort.h"
#include "transposition_table.h"

int hit = 0;
int miss = 0;

SearchResult min_max(BoardState *board_state, BoardStack *stack, uint8_t max_depth, uint8_t depth, BoardScore alpha, BoardScore beta, clock_t start, double seconds)
{
    if (has_timed_out(start, seconds))
        return (SearchResult){(BoardScore){0, UNKNOWN, 0}, false};
    push_game_history(board_state->board);
    if (threefold_repetition())
    {
        BoardScore score = score_board(board_state, depth, false);
        pop_game_history();
        score.result = DRAW;
        return (SearchResult){score, true};
    }

    TT_Entry entry = check_tt_entry(&board_state->board, max_depth - depth); // Threefold repetition might be wrong because of this
    if (entry.valid)
    {
        hit++;
        if (entry.type == EXACT)
        {
            pop_game_history();
            return (SearchResult){entry.score, true};
        }
        else
        {
            if (board_state->board.side_to_move == WHITE)
                alpha = max_score(alpha, entry.score, WHITE);
            else
                beta = max_score(beta, entry.score, BLACK);

            if (is_better_equal(alpha, beta, WHITE))
            {
                pop_game_history();
                return (SearchResult){entry.score, true};
            }
        }
    }
    else
    {
        miss++;
    }
    // if ((hit + miss) % 100000 == 0)
    // {
    //     printf("Hit: %d, Miss: %d\n", hit, miss);
    //     printf("Hit: %f, Miss: %f\n\n", (float)hit / (hit + miss) * 100, (float)miss / (hit + miss) * 100);
    // }

    // if in check - extend search
    if (depth == max_depth && (board_state->white_check || board_state->black_check))
        max_depth++;

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

        update_tt_entry(&board_state->board, score, 0, EXACT);

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
        pop_game_history();
        update_tt_entry(&board_state->board, score, max_depth - depth, EXACT);
        return (SearchResult){score, true};
    }

    sort_moves(board_state, stack, base);

    uint8_t depth_remaining = max_depth - depth;
    BoardScore best_score = get_worst_score(board_state->board.side_to_move);
    for (uint16_t i = base; i < stack->count; i++)
    {
        BoardState *next_board_state = &stack->boards[i];
        SearchResult search_result = min_max(next_board_state, stack, max_depth, depth + 1, alpha, beta, start, seconds);
        if (!search_result.valid)
        {
            stack->count = base;
            pop_game_history();
            update_tt_entry(&board_state->board, best_score, depth_remaining, NOT_EXACT);
            return (SearchResult){best_score, false};
        }

        BoardScore score = search_result.board_score;
        if (has_won(score.result, board_state->board.side_to_move))
        {
            stack->count = base;
            pop_game_history();
            update_tt_entry(&board_state->board, score, depth_remaining, EXACT);
            return (SearchResult){score, true};
        }

        best_score = max_score(best_score, score, board_state->board.side_to_move);
        if (board_state->board.side_to_move == WHITE)
            alpha = max_score(alpha, score, WHITE);
        else
            beta = max_score(beta, score, BLACK);

        if (is_better_equal(alpha, beta, WHITE)) // alpha >= beta
        {
            stack->count = base;
            pop_game_history();
            update_tt_entry(&board_state->board, best_score, depth_remaining, NOT_EXACT);
            return (SearchResult){best_score, true};
        }
    }

    stack->count = base;
    pop_game_history();
    update_tt_entry(&board_state->board, best_score, depth_remaining, EXACT);
    return (SearchResult){best_score, true};
}

void print_search_result(SearchResult search_result)
{
    if (search_result.valid)
        print_score(search_result.board_score);
    else
        printf("Invalid\n");
}