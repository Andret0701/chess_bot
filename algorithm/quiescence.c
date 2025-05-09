#include "quiescence.h"
#include "heuristic/heuristic.h"

QuiescenceResult quiescence(BoardState *board_state,
                            BoardStack *stack,
                            double alpha,
                            double beta,
                            uint8_t depth,
                            clock_t start,
                            double seconds)
{
    if (has_timed_out(start, seconds))
        return (QuiescenceResult){0, INVALID};

    // 1) Stand-pat
    double stand_pat = score_board(board_state, depth, false).score;
    double best_score = stand_pat;

    // 2) β-cutoff on stand-pat
    if (stand_pat >= beta)
        return (QuiescenceResult){stand_pat, VALID};

    if (stand_pat > alpha)
        alpha = stand_pat;

    // 4) Recurse on captures
    uint16_t base = stack->count;
    generate_captures(board_state, stack);

    for (uint16_t i = base; i < stack->count; i++)
    {
        BoardState *child = &stack->boards[i];
        QuiescenceResult quiescence_result = quiescence(child, stack, -beta, -alpha, depth + 1, start, seconds);
        quiescence_result.score = -quiescence_result.score;
        if (quiescence_result.valid == INVALID)
        {
            stack->count = base;
            return (QuiescenceResult){0, INVALID};
        }

        double score = quiescence_result.score;

        if (score >= beta)
        {
            stack->count = base;
            return (QuiescenceResult){score, VALID};
        }

        if (score > best_score)
            best_score = score;

        if (score > alpha)
            alpha = score;
    }

    stack->count = base;
    return (QuiescenceResult){best_score, VALID};
}
