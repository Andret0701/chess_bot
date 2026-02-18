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
    if (board_state->board.side_to_move == WHITE)
    {
        uint64_t non_pawn_white = board_state->board.white_pieces.knights |
                                  board_state->board.white_pieces.bishops |
                                  board_state->board.white_pieces.rooks |
                                  board_state->board.white_pieces.queens;

        return non_pawn_white != 0;
    }
    uint64_t non_pawn_black = board_state->board.black_pieces.knights |
                              board_state->board.black_pieces.bishops |
                              board_state->board.black_pieces.rooks |
                              board_state->board.black_pieces.queens;

    return non_pawn_black != 0;
}

int32_t nega_scout(BoardState *board_state, BoardStack *stack, uint8_t max_depth, uint8_t depth, int32_t alpha, int32_t beta, bool use_max_time, clock_t start, double seconds, bool use_max_nodes, uint64_t *nodes_searched, uint64_t max_nodes, bool allow_null_move, bool *search_cancelled)
{
    if (use_max_nodes && *nodes_searched >= max_nodes)
    {
        *search_cancelled = true;
        return 0;
    }
    (*nodes_searched)++;

    if (use_max_time && has_timed_out(start, seconds))
    {
        *search_cancelled = true;
        return 0;
    }

    uint64_t hash = hash_board(&board_state->board);
    TT_prefetch(hash);

    push_game_history(hash);
    if (threefold_repetition())
    {
        pop_game_history(hash);
        return 0;
    }

    if (depth > max_depth)
        max_depth = depth;
    uint8_t remaining_depth = max_depth - depth;

    int32_t alpha_orig = alpha; // Save original alpha
    TT_Entry tt_entry;
    bool found_tt = TT_lookup(hash, &tt_entry);

    // TT cutoff with proper bounds checking
    if (found_tt && tt_entry.depth >= remaining_depth)
    {
        int32_t tt_score = value_from_tt(tt_entry.score, depth);
        if (tt_entry.type == EXACT)
        {
            pop_game_history(hash);
            return tt_score;
        }
        else if (tt_entry.type == LOWERBOUND && tt_score >= beta)
        {
            pop_game_history(hash);
            return tt_score;
        }
        else if (tt_entry.type == UPPERBOUND && tt_score <= alpha)
        {
            pop_game_history(hash);
            return tt_score;
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

        int32_t child_alpha = -beta;
        int32_t child_beta = child_alpha + 1;

        int32_t null_search_score = -nega_scout(&null_board_state, stack, null_max_depth, depth + 1, child_alpha, child_beta, use_max_time, start, seconds, use_max_nodes, nodes_searched, max_nodes, false, search_cancelled);
        if (*search_cancelled)
        {
            pop_game_history(hash);
            return 0;
        }

        /* Fail-high?  => prune. */
        if (null_search_score >= beta)
        {
            pop_game_history(hash);
            return null_search_score;
        }
    }
    // =============== END NULL MOVE PRUNING ===============

    if (depth >= max_depth)
    {
        bool finished = is_finished(board_state);
        Result result = get_result(board_state, finished);
        finished |= result != UNKNOWN;

        int32_t score;
        if (!finished)
        {
            score = quiescence(board_state, stack, alpha_orig, beta, depth, 0, nodes_searched);
            if (use_max_nodes && *nodes_searched > max_nodes)
            {
                pop_game_history(hash);
                *search_cancelled = true;
                return 0;
            }
        }
        else
            score = get_result_score(result, depth);

        pop_game_history(hash);
        TT_store(hash, 0, value_to_tt(score, depth), EXACT, 0);
        return score;
    }

    uint16_t base = stack->count;
    generate_moves(board_state, stack);

    bool finished = base == stack->count;
    Result result = get_result(board_state, finished);
    finished |= result != UNKNOWN;
    if (finished)
    {
        int32_t score = get_result_score(result, depth);
        stack->count = base;
        pop_game_history(hash);
        TT_store(hash, 0, value_to_tt(score, depth), EXACT, 0);
        return score;
    }

    sort_moves(board_state, stack, base, found_tt ? tt_entry.move : 0, depth);

    int32_t best_score = WORST_SCORE;
    uint16_t best_move = 0;
    for (uint16_t i = base; i < stack->count; i++)
    {
        uint16_t move_number = i - base;
        bool first_move = move_number == 0;

        BoardState *next_board_state = &stack->boards[i];

        uint8_t extension = 0;
        bool is_check = is_move_check(next_board_state);
        bool is_threatening_promo = is_move_threatening_promotion(board_state, next_board_state);
        if (is_check || is_threatening_promo)
            extension = 1;

        uint8_t new_max_depth = max_depth + extension;

        int32_t score;
        if (first_move)
        {
            score = -nega_scout(next_board_state, stack, new_max_depth, depth + 1, -beta, -alpha, use_max_time, start, seconds, use_max_nodes, nodes_searched, max_nodes, allow_null_move, search_cancelled);
            if (*search_cancelled)
                goto cancelled;
        }
        else
        {
            uint8_t reduction = calculate_reduction(board_state, next_board_state, remaining_depth, move_number);
            // Null window search
            uint8_t reduced_max_depth = 0;
            if (new_max_depth > reduction)
                reduced_max_depth = new_max_depth - reduction;
            // check second alpha here
            score = -nega_scout(next_board_state, stack, reduced_max_depth, depth + 1, -(alpha + 1), -alpha, use_max_time, start, seconds, use_max_nodes, nodes_searched, max_nodes, allow_null_move, search_cancelled);
            if (*search_cancelled)
                goto cancelled;

            bool alpha_cutoff = score > alpha;
            if ((alpha_cutoff && score < beta) || ((reduction != 0) && alpha_cutoff))
            {
                score = -nega_scout(next_board_state, stack, new_max_depth, depth + 1, -beta, -alpha, use_max_time, start, seconds, use_max_nodes, nodes_searched, max_nodes, allow_null_move, search_cancelled);
                if (*search_cancelled)
                    goto cancelled;
            }
        }

        if (score > best_score)
        {
            best_score = score;
            best_move = next_board_state->move;
        }
        if (best_score > alpha)
            alpha = best_score;
        if (alpha >= beta)
        {
            if (!is_move_capture(board_state, next_board_state))
                add_killer_move(next_board_state->move, depth);
            break; // Beta cutoff
        }
    }

    stack->count = base;
    pop_game_history(hash);

    // Determine TT entry type
    TT_Entry_Type type;
    if (best_score <= alpha_orig)
    {
        type = UPPERBOUND;
    }
    else if (best_score >= beta)
    {
        type = LOWERBOUND;
    }
    else
    {
        type = EXACT;
    }

    TT_store(hash, remaining_depth, value_to_tt(best_score, depth), type, best_move);

    return best_score;

cancelled:
    stack->count = base;
    pop_game_history(hash);
    return 0;
}