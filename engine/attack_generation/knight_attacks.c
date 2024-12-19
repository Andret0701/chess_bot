#include "attack_generation.h"

uint64_t generate_knight_attacks(uint8_t x, uint8_t y)
{
    static const int knight_moves[8][2] = {
        {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

    uint64_t attacks = 0;

    for (int i = 0; i < 8; ++i)
    {
        int new_x = x + knight_moves[i][0];
        int new_y = y + knight_moves[i][1];

        if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
        {
            attacks |= position_to_u64(new_x, new_y);
        }
    }

    return attacks;
}