#include "min_max.h"
#include "heuristic.h"

SearchResult quiesce(BoardState *board_state, BoardStack *stack, BoardScore alpha, BoardScore beta,
                     uint8_t depth, clock_t start, double seconds, Color bot_color)
{
    if (has_timed_out(start, seconds))
        return (SearchResult){(BoardScore){0, UNKNOWN, 0}, false};

    // Get stand-pat score
    BoardScore stand_pat = score_board(board_state, depth, false);

    // Beta cutoff
    if (is_better_equal(stand_pat, beta, board_state->board.side_to_move, bot_color))
        return (SearchResult){stand_pat, true};

    // Update alpha if standing pat is better
    if (board_state->board.side_to_move == WHITE)
        alpha = max_score(alpha, stand_pat, WHITE, bot_color);
    else
        beta = max_score(beta, stand_pat, BLACK, bot_color);

    uint16_t base = stack->count;
    generate_captures(board_state, stack);

    BoardScore best_score = stand_pat; // Use stand-pat as initial best score

    for (uint16_t i = base; i < stack->count; i++)
    {
        BoardState *next_board_state = &stack->boards[i];

        // Recursive quiescence search
        SearchResult search_result = quiesce(next_board_state, stack, alpha, beta,
                                             depth + 1, start, seconds, bot_color);

        if (!search_result.valid)
        {
            stack->count = base;
            return (SearchResult){best_score, false};
        }

        BoardScore score = search_result.board_score;
        best_score = max_score(best_score, score, board_state->board.side_to_move, bot_color);

        if (board_state->board.side_to_move == WHITE)
            alpha = max_score(alpha, score, WHITE, bot_color);
        else
            beta = max_score(beta, score, BLACK, bot_color);

        if (is_better_equal(alpha, beta, WHITE, bot_color))
        {
            stack->count = base;
            return (SearchResult){best_score, true};
        }
    }

    stack->count = base;
    return (SearchResult){best_score, true};
}