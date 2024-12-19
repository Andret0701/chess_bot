#include "attack_generation.h"

uint64_t generate_black_pawn_attacks(uint8_t x, uint8_t y)
{
    uint64_t attacks = 0;
    if (x > 0)
        attacks |= position_to_u64(x - 1, y - 1);
    if (x < 7)
        attacks |= position_to_u64(x + 1, y - 1);
    return attacks;
}