
#include "negamax.h"
#include "heuristic/heuristic.h"
#include "game_history.h"
#include "../utils/utils.h"
#include "../engine/piece_moves.h"
#include <stdio.h>
#include "move_sort.h"
#include "quiescence.h"
#include "move_categorization.h"

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

    if (depth >= max_depth)
    {
        bool finished = is_finished(board_state);
        Result result = get_result(board_state, finished);
        finished |= result != UNKNOWN;

        BoardScore score;
        if (!finished)
        {
            QuiescenceResult quiescence_result = quiescence(board_state, stack, alpha.score, beta.score, depth, start, seconds);
            if (quiescence_result.valid == INVALID)
            {
                pop_game_history();
                return (SearchResult){(BoardScore){0, UNKNOWN, 0}, INVALID};
            }
            score = (BoardScore){quiescence_result.score, result, depth};
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

        // Classify the move
        bool is_capture = is_move_capture(board_state, next_board_state);
        bool is_promo = is_move_promotion(board_state, next_board_state);
        bool is_check = is_move_check(next_board_state);
        bool is_quiet = !is_capture && !is_promo && !is_check;
        bool is_threatening_promo = is_move_threatening_promotion(board_state, next_board_state);

        int reduction = 0;
        bool do_reduction = is_quiet && (i >= base + 2) && (depth >= 3);
        if (do_reduction)
            reduction = 1; // Reduce depth by 1

        int extension = 0;
        if (is_check || is_threatening_promo)
            extension = 1; // Extend depth by 1

        SearchResult search_result = negamax(next_board_state, stack, (max_depth + extension) - reduction, depth + 1, invert_score(beta), invert_score(alpha), start, seconds);
        search_result.board_score = invert_score(search_result.board_score);
        if (search_result.valid == INVALID)
        {
            stack->count = base;
            pop_game_history();
            return (SearchResult){(BoardScore){0, UNKNOWN, 0}, INVALID};
        }

        if (do_reduction && is_greater_score(search_result.board_score, alpha))
        {
            // If the score is greater than or equal to beta, we can reduce the depth
            search_result = negamax(
                next_board_state, stack, max_depth + extension, depth + 1,
                invert_score(beta), invert_score(alpha), start, seconds);
            search_result.board_score = invert_score(search_result.board_score);
            if (search_result.valid == INVALID)
            {
                stack->count = base;
                pop_game_history();
                return (SearchResult){(BoardScore){0, UNKNOWN, 0}, INVALID};
            }
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
