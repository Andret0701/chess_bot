#include "quiescence.h"
#include "heuristic/heuristic.h"
#include "transposition_table.h"
#include "zobrist_hash.h"
#include "../engine/piece_moves.h"
#include "move_sort.h"

#define MAX_QUIESCENCE_DEPTH 6

int32_t quiescence(BoardState *board_state,
                   BoardStack *stack,
                   int32_t alpha,
                   int32_t beta,
                   uint8_t depth,
                   uint8_t depth_in_quiescence, uint64_t *nodes_searched)
{
    (*nodes_searched)++;
    if (depth_in_quiescence >= MAX_QUIESCENCE_DEPTH)
        return score_board(board_state);

    uint64_t hash = hash_board(&board_state->board);
    TT_Entry tt_entry;
    if (TT_lookup(hash, &tt_entry))
    {
        int32_t tt_score = value_from_tt(tt_entry.score, depth);
        if (tt_entry.type == EXACT)
            return tt_score;

        if (tt_entry.type == LOWERBOUND && tt_score >= beta)
            return tt_score;

        if (tt_entry.type == UPPERBOUND && tt_score <= alpha)
            return tt_score;
    }

    // 1) Stand-pat
    int32_t stand_pat = score_board(board_state);
    int32_t best_score = stand_pat;

    // 2) β-cutoff on stand-pat
    if (stand_pat >= beta)
        return stand_pat;

    if (stand_pat > alpha)
        alpha = stand_pat;

    // 4) Recurse on captures
    uint16_t base = stack->count;
    generate_captures(board_state, stack);
    sort_moves(board_state, stack, base, 0);

    for (uint16_t i = base; i < stack->count; i++)
    {
        BoardState *child = &stack->boards[i];
        int32_t score = -quiescence(child, stack, -beta, -alpha, depth + 1, depth_in_quiescence + 1, nodes_searched);

        if (score >= beta)
        {
            stack->count = base;
            return score;
        }

        if (score > best_score)
            best_score = score;

        if (score > alpha)
            alpha = score;
    }

    stack->count = base;
    return best_score;
}
