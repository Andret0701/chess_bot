#include "quiescence.h"
#include "heuristic/heuristic.h"
#include "transposition_table.h"
#include "zobrist_hash.h"
#include "../engine/piece_moves.h"
#include "move_sort.h"

#define MAX_QUIESCENCE_DEPTH 6

double quiescence(BoardState *board_state,
                  BoardStack *stack,
                  double alpha,
                  double beta,
                  int depth_in_quiescence)
{
    if (depth_in_quiescence >= MAX_QUIESCENCE_DEPTH)
        return score_board(board_state);

    uint64_t hash = hash_board(&board_state->board);
    TT_prefetch(hash);

    // 1) Stand-pat
    double stand_pat = score_board(board_state);
    double best_score = stand_pat;

    // 2) β-cutoff on stand-pat
    if (stand_pat >= beta)
        return stand_pat;

    if (stand_pat > alpha)
        alpha = stand_pat;

    // 4) Recurse on captures
    uint16_t base = stack->count;
    generate_captures(board_state, stack);
    sort_moves(board_state, stack, base);

    TT_Entry tt_entry;
    if (TT_lookup(hash, &tt_entry))
    {
        if (tt_entry.type == EXACT)
            return tt_entry.score;

        if (tt_entry.type == LOWERBOUND && tt_entry.score >= beta)
            return tt_entry.score;

        if (tt_entry.type == UPPERBOUND && tt_entry.score <= alpha)
            return tt_entry.score;
    }

    for (uint16_t i = base; i < stack->count; i++)
    {
        BoardState *child = &stack->boards[i];
        double score = -quiescence(child, stack, -beta, -alpha, depth_in_quiescence + 1);

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
