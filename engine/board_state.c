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
    bool is_finished = stack->count == 0;
    destroy_board_stack(stack);

    if (is_finished)
    {
        if (board_state->board.side_to_move == WHITE)
            return board_state->white_check ? LOST : DRAW;
        else
            return board_state->black_check ? LOST : DRAW;
    }
    return UNKNOWN;
}