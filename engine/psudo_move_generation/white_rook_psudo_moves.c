#include "../piece_moves.h"

void generate_white_rook_psudo_moves(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    static const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (int d = 0; d < 4; ++d)
    {
        int dx = directions[d][0];
        int dy = directions[d][1];

        for (int i = 1; i < 8; ++i)
        {
            int new_x = x + i * dx;
            int new_y = y + i * dy;

            if (new_x < 0 || new_x >= 8 || new_y < 0 || new_y >= 8)
                break;

            if (is_white_piece(board_state, new_x, new_y))
                break;

            stack->moves[stack->count++] = new_simple_encoded_move(
                position_to_index(x, y),
                position_to_index(new_x, new_y),
                is_black_piece(board_state, new_x, new_y));

            if (is_black_piece(board_state, new_x, new_y))
                break;
        }
    }
}