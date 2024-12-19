#include "attack_generation.h"

uint64_t generate_king_attacks(uint8_t x, uint8_t y)
{
    uint64_t attacks = 0;

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
                attacks |= position_to_u64(new_x, new_y);
            }
        }
    }

    return attacks;
}