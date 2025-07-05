#include "null_move.h"

BoardState apply_null_move(BoardState *board_state)
{
    BoardState null_board_state = *board_state;
    null_board_state.board.side_to_move = (board_state->board.side_to_move == WHITE ? BLACK : WHITE);
    null_board_state.board.en_passant = 0;
    return null_board_state;
}