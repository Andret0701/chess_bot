#include "quiescence.h"
#include "heuristic/heuristic.h"
#include "transposition_table.h"
#include "zobrist_hash.h"
#include "../engine/piece_moves.h"
#include "move_sort.h"
#include "algorithm/heuristic/heuristic.h"
#include "algorithm/move_categorization.h"

#define MAX_QUIESCENCE_DEPTH 6

int32_t quiescence(BoardState *board_state,
                   BoardStack *stack,
                   int32_t alpha,
                   int32_t beta,
                   uint8_t depth,
                   uint8_t depth_in_quiescence, uint64_t *nodes_searched)
{
    (*nodes_searched)++;

    bool in_check = is_move_check(board_state);

    /*
     * A position in check must search an evasion, even when the
     * normal quiescence depth limit has been reached.
     */
    if (depth_in_quiescence >= MAX_QUIESCENCE_DEPTH && !in_check)
        return score_board(board_state);

    uint64_t hash = hash_board(&board_state->board);
    TT_Entry tt_entry;
    bool tt_hit = TT_lookup(hash, &tt_entry);
    // if (tt_hit && is_mate_score(tt_entry.score))
    //     tt_hit = false; // Don't use mate scores from quiescence search

    if (tt_hit)
    {
        int32_t tt_score = value_from_tt(tt_entry.score, depth);

        if (tt_entry.type == EXACT)
            return tt_score;

        if (tt_entry.type == LOWERBOUND)
        {
            if (tt_score > alpha)
                alpha = tt_score;
        }
        else if (tt_entry.type == UPPERBOUND)
        {
            if (tt_score < beta)
                beta = tt_score;
        }

        if (alpha >= beta)
            return tt_score;
    }

    int32_t best_score;

    if (!in_check)
    {
        int32_t stand_pat = score_board(board_state);
        best_score = stand_pat;

        if (stand_pat >= beta)
            return stand_pat;

        if (stand_pat > alpha)
            alpha = stand_pat;
    }
    else
    {
        best_score = WORST_SCORE;
    }

    uint16_t base = stack->count;

    BoardState *next_board_state = NULL;
    if (in_check)
    {
        generate_moves(board_state, stack);
        next_board_state = sort_moves(board_state, stack, base, tt_hit ? tt_entry.move : 0, depth);
    }
    else
    {
        generate_captures(board_state, stack);
        next_board_state = sort_moves_q(board_state, stack, base);
    }

    /*
     * No legal evasion means checkmate.
     */
    if (in_check && stack->count == base)
    {
        stack->count = base;
        return -MATE_SCORE + depth;
    }
    else if (!in_check && stack->count == base)
    {
        bool finished = is_finished(board_state);
        Result result = get_result(board_state, finished);
        finished |= result != UNKNOWN;
        if (finished)
        {
            int32_t score = get_result_score(result, depth);
            stack->count = base;
            return score;
        }
    }

    while (next_board_state != NULL)
    {
        int32_t score = -quiescence(
            next_board_state,
            stack,
            -beta,
            -alpha,
            depth + 1,
            depth_in_quiescence + 1,
            nodes_searched);

        if (score > best_score)
            best_score = score;

        if (score > alpha)
            alpha = score;

        if (alpha >= beta)
        {
            stack->count = base;
            return best_score;
        }

        next_board_state = next_board_state->next;
    }

    stack->count = base;
    return best_score;
}
