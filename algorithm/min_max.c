#include "min_max.h"
#include "heuristic.h"

SearchResult min_max(BoardState *board_state, BoardStack *stack, uint8_t max_depth, uint8_t depth, BoardScore alpha, BoardScore beta, clock_t start, double seconds)
{
    if (has_timed_out(start, seconds))
        return (SearchResult){(BoardScore){0, UNKNOWN, 0}, false};

    uint16_t base = stack->count;
    generate_moves(board_state, stack);
    bool is_finished = stack->count == base;

    Result result = get_result(board_state, is_finished);
    is_finished |= result != UNKNOWN;
    if (depth == max_depth || is_finished)
    {
        stack->count = base;
        BoardScore score = score_board(board_state, depth, is_finished);
        return (SearchResult){score, true};
    }

    BoardScore best_score = get_worst_score(board_state->board.side_to_move);
    for (uint16_t i = base; i < stack->count; i++)
    {
        BoardState *next_board_state = &stack->boards[i];
        SearchResult search_result = min_max(next_board_state, stack, max_depth, depth + 1, alpha, beta, start, seconds);
        if (!search_result.valid)
        {
            stack->count = base;
            return (SearchResult){best_score, false};
        }

        BoardScore score = search_result.board_score;
        if (has_won(score.result, board_state->board.side_to_move))
        {
            stack->count = base;
            return (SearchResult){score, true};
        }

        best_score = max_score(best_score, score, board_state->board.side_to_move);
        if (board_state->board.side_to_move == WHITE)
            alpha = max_score(alpha, score, WHITE);
        else
            beta = max_score(beta, score, BLACK);

        if (is_better_equal(alpha, beta, WHITE)) // alpha >= beta
        {
            stack->count = base;
            return (SearchResult){best_score, true};
        }
    }

    stack->count = base;
    return (SearchResult){best_score, true};
}

void print_search_result(SearchResult search_result)
{
    if (search_result.valid)
        print_score(search_result.board_score);
    else
        printf("Invalid\n");
}