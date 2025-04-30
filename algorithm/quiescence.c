#include "quiescence.h"
#include "heuristic/heuristic.h"

SearchResult quiescence(BoardState *board_state,
                        BoardStack *stack,
                        BoardScore alpha,
                        BoardScore beta,
                        uint8_t depth,
                        clock_t start,
                        double seconds)
{
    if (has_timed_out(start, seconds))
        return (SearchResult){{0, UNKNOWN, 0}, INVALID};

    Color side = board_state->board.side_to_move;

    // 1) Stand-pat
    BoardScore stand_pat = score_board(board_state, depth, false);
    BoardScore best_score = stand_pat;

    // 2) β-cutoff on stand-pat
    if (is_greater_equal_score(stand_pat, beta))
        return (SearchResult){stand_pat, VALID};

    alpha = max_score(alpha, stand_pat);

    // 4) Recurse on captures
    uint16_t base = stack->count;
    generate_captures(board_state, stack);

    for (uint16_t i = base; i < stack->count; i++)
    {
        BoardState *child = &stack->boards[i];
        SearchResult sr = quiescence(child, stack, invert_score(beta), invert_score(alpha), depth + 1, start, seconds);
        sr.board_score = invert_score(sr.board_score);

        if (sr.valid == INVALID)
        {
            stack->count = base;
            return (SearchResult){{0, UNKNOWN, 0}, INVALID};
        }

        BoardScore score = sr.board_score;

        // 4a) Captured‐move cutoff
        if (is_greater_equal_score(score, beta))
        {
            stack->count = base;
            return (SearchResult){score, VALID};
        }

        best_score = max_score(best_score, score);
        alpha = max_score(alpha, score);
    }

    stack->count = base;
    return (SearchResult){best_score, VALID};
}
