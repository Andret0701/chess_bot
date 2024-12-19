#include "attack_generation.h"

uint64_t generate_rook_attacks(BoardState *board_state, uint8_t x, uint8_t y)
{
    uint64_t attacks = 0;

    // Loop to check moves in the N direction
    for (uint8_t i = y + 1; i < 8; i++)
    {
        uint64_t target_pos = position_to_u64(x, i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                attacks |= target_pos;
            }

            break;
        }

        attacks |= target_pos;
    }

    // Loop to check moves in the S direction
    for (uint8_t i = y - 1; i < 8; i--)
    {
        uint64_t target_pos = position_to_u64(x, i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                attacks |= target_pos;
            }

            break;
        }

        attacks |= target_pos;
    }

    // Loop to check moves in the E direction
    for (uint8_t i = x + 1; i < 8; i++)
    {
        uint64_t target_pos = position_to_u64(i, y);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                attacks |= target_pos;
            }

            break;
        }

        attacks |= target_pos;
    }

    // Loop to check moves in the W direction
    for (uint8_t i = x - 1; i < 8; i--)
    {
        uint64_t target_pos = position_to_u64(i, y);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                attacks |= target_pos;
            }

            break;
        }

        attacks |= target_pos;
    }

    return attacks;
}