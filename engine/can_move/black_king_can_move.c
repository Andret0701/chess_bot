#include "../piece_moves.h"

bool generate_black_king_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    BoardState new_board_state = {0};
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;
            int new_x = x + dx;
            int new_y = y + dy;

            if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
            {
                if (board_state->black_pieces & position_to_u64(new_x, new_y))
                    continue;

                if (!(board_state->white_attack & position_to_u64(new_x, new_y)))
                    return true;
            }
        }
    }

    return false;
}
