#include "nega_scout.h"

#include "heuristic/heuristic.h"
#include "game_history.h"
#include "../utils/utils.h"
#include "../engine/piece_moves.h"
#include <stdio.h>
#include "move_sort.h"
#include "quiescence.h"
#include "move_categorization.h"
#include "transposition_table.h"
#include "zobrist_hash.h"
#include "null_move.h"
#include "reductions.h"
#include <math.h>

static bool has_non_pawn_material(BoardState *board_state)
{
    // Count non-pawn material for both sides
    uint64_t non_pawn_white = board_state->board.white_pieces.knights |
                              board_state->board.white_pieces.bishops |
                              board_state->board.white_pieces.rooks |
                              board_state->board.white_pieces.queens;
    uint64_t non_pawn_black = board_state->board.black_pieces.knights |
                              board_state->board.black_pieces.bishops |
                              board_state->board.black_pieces.rooks |
                              board_state->board.black_pieces.queens;

    return (non_pawn_white | non_pawn_black) != 0;
}

SearchResult nega_scout(BoardState *board_state, BoardStack *stack, uint8_t max_depth, uint8_t depth, BoardScore alpha, BoardScore beta, bool use_max_time, clock_t start, double seconds, bool allow_null_move)
{
    if (use_max_time && has_timed_out(start, seconds))
        return (SearchResult){(BoardScore){0, UNKNOWN, 0}, INVALID};

    uint64_t hash = hash_board(&board_state->board);
    TT_prefetch(hash);

    push_game_history(board_state->board);
    if (threefold_repetition())
    {
        BoardScore score = (BoardScore){0, THREEFOLD_REPETITION, depth};
        pop_game_history();
        return (SearchResult){score, VALID};
    }
    if (has_50_move_rule_occurred())
    {
        BoardScore score = (BoardScore){0, FIFTY_MOVE_RULE, depth};
        pop_game_history();
        return (SearchResult){score, VALID};
    }

    if (depth > max_depth)
        max_depth = depth;
    uint8_t remaining_depth = max_depth - depth;

    BoardScore alpha_orig = alpha; // Save original alpha
    TT_Entry tt_entry;
    bool found_tt = TT_lookup(hash, &tt_entry);

    // TT cutoff with proper bounds checking
    if (found_tt && tt_entry.depth >= remaining_depth)
    {
        BoardScore tt_score = {tt_entry.score, tt_entry.result, depth + tt_entry.depth};

        if (tt_entry.type == EXACT)
        {
            pop_game_history();
            return (SearchResult){tt_score, VALID};
        }
        else if (tt_entry.type == LOWERBOUND && is_greater_equal_score(tt_score, beta))
        {
            pop_game_history();
            return (SearchResult){tt_score, VALID};
        }
        else if (tt_entry.type == UPPERBOUND && is_less_equal_score(tt_score, alpha))
        {
            pop_game_history();
            return (SearchResult){tt_score, VALID};
        }
    }

    uint8_t NULL_MOVE_R = 3;
    if (allow_null_move &&
        remaining_depth >= NULL_MOVE_R &&
        !board_state->white_check && !board_state->black_check &&
        has_non_pawn_material(board_state))
    {
        BoardState null_board_state = apply_null_move(board_state);
        uint8_t null_max_depth = max_depth - NULL_MOVE_R;

        BoardScore child_alpha = invert_score(beta);
        BoardScore child_beta = child_alpha;
        child_beta.score += 1; // Set child alpha to beta - 1

        SearchResult null_search_result = nega_scout(&null_board_state, stack, null_max_depth, depth + 1, child_alpha, child_beta, use_max_time, start, seconds, false);
        null_search_result.board_score = invert_score(null_search_result.board_score);
        if (null_search_result.valid == INVALID)
        {
            pop_game_history();
            return (SearchResult){(BoardScore){0, UNKNOWN, 0}, INVALID};
        }

        /* Fail-high?  => prune. */
        if (is_greater_equal_score(null_search_result.board_score, beta))
        {
            pop_game_history();
            return (SearchResult){null_search_result.board_score, VALID};
        }
    }
    // =============== END NULL MOVE PRUNING ===============

    if (depth >= max_depth)
    {
        bool finished = is_finished(board_state);
        Result result = get_result(board_state, finished);
        finished |= result != UNKNOWN;

        BoardScore score;
        if (!finished)
        {
            double quiescence_score = quiescence(board_state, stack, alpha.score, beta.score, 0);
            score = (BoardScore){quiescence_score, UNKNOWN, depth};
        }
        else
            score = (BoardScore){0, result, depth};

        pop_game_history();
        TT_store(hash, 0, score.score, result, EXACT, 0);
        return (SearchResult){score, VALID};
    }

    uint16_t base = stack->count;
    generate_moves(board_state, stack);

    bool finished = base == stack->count;
    Result result = get_result(board_state, finished);
    finished |= result != UNKNOWN;
    if (finished)
    {
        BoardScore score = (BoardScore){0, result, depth};
        stack->count = base;
        pop_game_history();
        return (SearchResult){score, VALID};
    }

    if (found_tt)
        sort_moves_tt(board_state, stack, base, tt_entry.move);
    else
        sort_moves(board_state, stack, base);

    BoardScore best_score = WORST_SCORE;
    uint16_t best_move = 0;
    for (uint16_t i = base; i < stack->count; i++)
    {
        uint16_t move_number = i - base;
        bool first_move = move_number == 0;

        BoardState *next_board_state = &stack->boards[i];

        int extension = 0;
        bool is_check = is_move_check(next_board_state);
        bool is_threatening_promo = is_move_threatening_promotion(board_state, next_board_state);
        if (is_check || is_threatening_promo)
            extension = 1;

        SearchResult search_result;
        if (first_move)
        {
            search_result = nega_scout(next_board_state, stack, max_depth + extension, depth + 1, invert_score(beta), invert_score(alpha), use_max_time, start, seconds, allow_null_move);
            search_result.board_score = invert_score(search_result.board_score);
            if (search_result.valid == INVALID)
                goto invalid;
        }
        else
        {
            uint8_t reduction = calculate_reduction(board_state, next_board_state, remaining_depth, move_number);
            // Null window search
            int new_max_depth = max_depth + extension - reduction;
            if (new_max_depth < 0)
                new_max_depth = 0;
            search_result = nega_scout(next_board_state, stack, new_max_depth, depth + 1, invert_score((BoardScore){alpha.score + 1, alpha.result, alpha.depth}), invert_score(alpha), use_max_time, start, seconds, allow_null_move);
            search_result.board_score = invert_score(search_result.board_score);
            if (search_result.valid == INVALID)
                goto invalid;

            bool alpha_cutoff = is_greater_score(search_result.board_score, alpha);
            if ((alpha_cutoff && is_less_score(search_result.board_score, beta)) || ((reduction != 0) && alpha_cutoff))
            {
                search_result = nega_scout(next_board_state, stack, max_depth + extension, depth + 1, invert_score(beta), invert_score(alpha), use_max_time, start, seconds, allow_null_move);
                search_result.board_score = invert_score(search_result.board_score);
                if (search_result.valid == INVALID)
                    goto invalid;
            }
        }

        BoardScore score = search_result.board_score;

        if (is_greater_score(score, best_score))
        {
            best_score = score;
            best_move = next_board_state->move;
        }
        alpha = max_score(alpha, score);
        if (is_greater_equal_score(alpha, beta))
        {
            break; // Beta cutoff
        }
    }

    stack->count = base;
    pop_game_history();

    // Determine TT entry type
    TT_Entry_Type type;
    if (is_less_equal_score(best_score, alpha_orig))
    {
        type = UPPERBOUND;
    }
    else if (is_greater_equal_score(best_score, beta))
    {
        type = LOWERBOUND;
    }
    else
    {
        type = EXACT;
    }

    TT_store(hash, remaining_depth, best_score.score,
             best_score.result, type, best_move);

    return (SearchResult){best_score, VALID};

invalid:
    stack->count = base;
    pop_game_history();
    return (SearchResult){(BoardScore){0, UNKNOWN, 0}, INVALID};
}