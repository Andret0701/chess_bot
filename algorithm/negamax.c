
#include "negamax.h"
#include "heuristic/heuristic.h"
#include "game_history.h"
#include "../utils/utils.h"
#include "../engine/piece_moves.h"
#include <stdio.h>
#include "move_sort.h"
#include "quiescence.h"

SearchResult negamax(BoardState *board_state, BoardStack *stack, uint8_t max_depth, uint8_t depth, BoardScore alpha, BoardScore beta, clock_t start, double seconds)
{
    if (has_timed_out(start, seconds))
        return (SearchResult){(BoardScore){0, UNKNOWN, 0}, INVALID};

    push_game_history(board_state->board);
    if (threefold_repetition() || has_50_move_rule_occurred())
    {
        BoardScore score = score_board(board_state, depth, false);
        pop_game_history();
        score.result = DRAW;
        return (SearchResult){score, VALID};
    }

    // if in check - extend search
    if (board_state->white_check || board_state->black_check)
        max_depth++;

    if (depth == max_depth)
    {
        bool finished = is_finished(board_state);
        Result result = get_result(board_state, finished);
        finished |= result != UNKNOWN;

        BoardScore score;
        if (!finished)
        {
            SearchResult q_result = quiescence(board_state, stack, alpha, beta, depth, start, seconds);
            if (q_result.valid == INVALID)
            {
                pop_game_history();
                return (SearchResult){(BoardScore){0, UNKNOWN, 0}, INVALID};
            }
            score = (BoardScore){q_result.board_score.score, result, depth};
        }
        else
            score = score_board(board_state, depth, finished);

        pop_game_history();
        return (SearchResult){score, VALID};
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
        return (SearchResult){score, VALID};
    }

    sort_moves(board_state, stack, base);

    BoardScore best_score = WORST_SCORE;
    for (uint16_t i = base; i < stack->count; i++)
    {
        BoardState *next_board_state = &stack->boards[i];
        SearchResult search_result = negamax(next_board_state, stack, max_depth, depth + 1, invert_score(beta), invert_score(alpha), start, seconds);
        search_result.board_score = invert_score(search_result.board_score);
        if (search_result.valid == INVALID)
        {
            stack->count = base;
            pop_game_history();
            return (SearchResult){(BoardScore){0, UNKNOWN, 0}, INVALID};
        }

        BoardScore score = search_result.board_score;
        best_score = max_score(best_score, score);
        alpha = max_score(alpha, score);
        if (is_greater_equal_score(alpha, beta))
        {
            stack->count = base;
            pop_game_history();
            return (SearchResult){best_score, VALID};
        }
    }

    stack->count = base;
    pop_game_history();
    return (SearchResult){best_score, VALID};
}
