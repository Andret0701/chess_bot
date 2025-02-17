#include "bitboard.h"
#include <stdio.h>

uint64_t flip_bitboard(uint64_t bitboard)
{
    bitboard = ((bitboard >> 8) & 0x00FF00FF00FF00FFULL) | ((bitboard & 0x00FF00FF00FF00FFULL) << 8);
    bitboard = ((bitboard >> 16) & 0x0000FFFF0000FFFFULL) | ((bitboard & 0x0000FFFF0000FFFFULL) << 16);
    bitboard = (bitboard >> 32) | (bitboard << 32);
    return bitboard;
}

void print_bitboard(uint64_t bitboard)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if (bitboard & (1ULL << square))
            {
                printf("[]");
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
    }

    printf("\n");
}

uint64_t position_to_bitboard(uint8_t x, uint8_t y)
{
    return 1ULL << (x + y * 8);
}

uint64_t move_bitboard_left(uint64_t bitboard)
{
    return (bitboard & ~FILE_H_MASK) << 1;
}

uint64_t move_bitboard_right(uint64_t bitboard)
{
    return (bitboard & ~FILE_A_MASK) >> 1;
}

uint64_t move_bitboard_up(uint64_t bitboard)
{
    return (bitboard & ~RANK_8_MASK) << 8;
}

uint64_t move_bitboard_down(uint64_t bitboard)
{
    return (bitboard & ~RANK_1_MASK) >> 8;
}

uint64_t expand_bitboard(uint64_t bitboard)
{
    uint64_t up = move_bitboard_up(bitboard);
    uint64_t down = move_bitboard_down(bitboard);
    uint64_t left = move_bitboard_left(bitboard);
    uint64_t right = move_bitboard_right(bitboard);
    uint64_t up_left = move_bitboard_left(up);
    uint64_t up_right = move_bitboard_right(up);
    uint64_t down_left = move_bitboard_left(down);
    uint64_t down_right = move_bitboard_right(down);

    return up | down | left | right | up_left | up_right | down_left | down_right;
}