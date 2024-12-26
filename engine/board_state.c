#include "board_state.h"
#include "piece_moves.h"
#include "board_stack.h"

BoardState board_to_board_state(Board *board)
{
    BoardState board_state = {0};
    copy_board(board, &board_state.board);
    init_board(&board_state);
    return board_state;
}

Result get_game_result(BoardState *board_state)
{
    BoardStack *stack = create_board_stack(65535);
    generate_moves(board_state, stack);

    if (stack->count == 0)
    {
        destroy_board_stack(stack);
        if (board_state->white_check)
            return BLACK_WON;
        else if (board_state->black_check)
            return WHITE_WON;
        else
            return DRAW;
    }

    destroy_board_stack(stack);
    return UNKNOWN;
}