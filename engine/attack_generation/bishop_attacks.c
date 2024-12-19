#include "attack_generation.h"

uint64_t generate_bishop_attacks(BoardState *board_state, uint8_t x, uint8_t y)
{
    uint64_t attacks = 0;

    // Loop to check moves in the NE direction
    for (uint8_t i = 1; x + i < 8 && y + i < 8; i++)
    {
        uint64_t target_pos = position_to_u64(x + i, y + i);
        if (board_state->occupied & target_pos)
        {
            attacks |= target_pos;
            break;
        }

        attacks |= target_pos;
    }

    // Loop to check moves in the SE direction
    for (uint8_t i = 1; x + i < 8 && y - i >= 0; i++)
    {
        uint64_t target_pos = position_to_u64(x + i, y - i);
        if (board_state->occupied & target_pos)
        {
            attacks |= target_pos;
            break;
        }

        attacks |= target_pos;
    }

    // Loop to check moves in the NW direction
    for (uint8_t i = 1; x - i >= 0 && y + i < 8; i++)
    {
        uint64_t target_pos = position_to_u64(x - i, y + i);
        if (board_state->occupied & target_pos)
        {
            attacks |= target_pos;
            break;
        }

        attacks |= target_pos;
    }

    // Loop to check moves in the SW direction
    for (uint8_t i = 1; x - i >= 0 && y - i >= 0; i++)
    {
        uint64_t target_pos = position_to_u64(x - i, y - i);
        if (board_state->occupied & target_pos)
        {
            attacks |= target_pos;
            break;
        }

        attacks |= target_pos;
    }

    return attacks;
}