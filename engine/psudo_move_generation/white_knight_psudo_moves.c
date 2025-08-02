#include "../piece_moves.h"

void generate_white_knight_psudo_moves(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    static const int knight_moves[8][2] = {
        {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

    for (int i = 0; i < 8; ++i)
    {
        int new_x = x + knight_moves[i][0];
        int new_y = y + knight_moves[i][1];

        if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
            if (!is_white_piece(board_state, new_x, new_y))
                stack->moves[stack->count++] = new_simple_encoded_move(position_to_index(x, y), position_to_index(new_x, new_y), is_black_piece(board_state, new_x, new_y));
    }
}
