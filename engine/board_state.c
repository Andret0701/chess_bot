#include "board_state.h"
#include "piece_moves.h"

BoardState board_to_board_state(Board *board)
{
    BoardState board_state = {0};
    copy_board(board, &board_state.board);
    init_board(&board_state);
    return board_state;
}