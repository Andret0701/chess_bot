#include "../piece_moves.h"

void generate_white_knight_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    static const int knight_moves[8][2] = {
        {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

    for (int i = 0; i < 8; ++i)
    {
        int new_x = x + knight_moves[i][0];
        int new_y = y + knight_moves[i][1];

        if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
        {
            if ((board_state->white_pieces & position_to_bitboard(new_x, new_y)) == 0)
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, new_x, new_y);
                new_board_state->board.white_pieces.knights &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.knights |= position_to_bitboard(new_x, new_y);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(new_x, new_y), is_black_piece(board_state, new_x, new_y));
                push_white_move(board_state, new_board_state, stack);
            }
        }
    }
}
