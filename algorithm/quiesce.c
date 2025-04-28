#include "min_max.h"
#include "heuristic/heuristic.h"

SearchResult quiesce(BoardState *board_state, BoardStack *stack, BoardScore alpha, BoardScore beta,
                     uint8_t depth, clock_t start, double seconds)
{
    if (has_timed_out(start, seconds))
        return (SearchResult){(BoardScore){0, UNKNOWN, 0}, false, 1};

    // Get stand-pat score
    BoardScore stand_pat = score_board(board_state, depth, false);

    // Beta cutoff
    if (is_better_equal(stand_pat, beta, board_state->board.side_to_move))
        return (SearchResult){stand_pat, true, 1};

    // Update alpha if standing pat is better
    if (board_state->board.side_to_move == WHITE)
        alpha = max_score(alpha, stand_pat, WHITE);
    else
        beta = max_score(beta, stand_pat, BLACK);

    uint16_t base = stack->count;
    generate_captures(board_state, stack);

    BoardScore best_score = stand_pat; // Use stand-pat as initial best score
    uint64_t nodes_searched = 0;

    for (uint16_t i = base; i < stack->count; i++)
    {
        BoardState *next_board_state = &stack->boards[i];

        // Recursive quiescence search
        SearchResult search_result = quiesce(next_board_state, stack, alpha, beta,
                                             depth + 1, start, seconds);
        nodes_searched += search_result.nodes_searched + 1; // +1 for the current node
        search_result.nodes_searched = nodes_searched;

        if (!search_result.valid)
        {
            stack->count = base;
            return (SearchResult){best_score, false, nodes_searched};
        }

        BoardScore score = search_result.board_score;
        best_score = max_score(best_score, score, board_state->board.side_to_move);

        if (board_state->board.side_to_move == WHITE)
            alpha = max_score(alpha, score, WHITE);
        else
            beta = max_score(beta, score, BLACK);

        if (is_better_equal(alpha, beta, WHITE))
        {
            stack->count = base;
            return (SearchResult){best_score, true, nodes_searched};
        }
    }

    stack->count = base;
    return (SearchResult){best_score, true, nodes_searched};
}