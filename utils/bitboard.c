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

uint64_t decrement_file(uint64_t bitboard)
{
    return (bitboard & ~FILE_A_MASK) >> 1;
}

uint64_t increment_file(uint64_t bitboard)
{
    return (bitboard & ~FILE_H_MASK) << 1;
}

uint64_t increment_rank(uint64_t bitboard)
{
    return (bitboard & ~RANK_8_MASK) << 8;
}

uint64_t decrement_rank(uint64_t bitboard)
{
    return (bitboard & ~RANK_1_MASK) >> 8;
}

uint64_t expand_bitboard(uint64_t bitboard)
{
    uint64_t up = increment_rank(bitboard);
    uint64_t down = decrement_rank(bitboard);
    uint64_t left = decrement_file(bitboard);
    uint64_t right = increment_file(bitboard);
    uint64_t up_left = decrement_file(up);
    uint64_t up_right = increment_file(up);
    uint64_t down_left = decrement_file(down);
    uint64_t down_right = increment_file(down);

    return up | down | left | right | up_left | up_right | down_left | down_right;
}

uint64_t get_passed_pawn_mask_white(uint64_t pawn)
{
    uint64_t mask = 0;
    mask |= increment_rank(pawn);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);

    mask |= decrement_file(mask);
    mask |= increment_file(mask);

    return mask;
}

uint64_t get_passed_pawn_mask_black(uint64_t pawn)
{
    uint64_t mask = 0;
    mask |= decrement_rank(pawn);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);

    mask |= decrement_file(mask);
    mask |= increment_file(mask);

    return mask;
}

uint64_t get_backward_pawn_mask_white(uint64_t pawn)
{
    uint64_t mask = decrement_file(pawn) | increment_file(pawn);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);

    return mask;
}

uint64_t get_backward_pawn_mask_black(uint64_t pawn)
{
    uint64_t mask = decrement_file(pawn) | increment_file(pawn);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);

    return mask;
}

uint64_t get_white_front_file_mask(uint64_t piece)
{
    uint64_t mask = increment_rank(piece);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    mask |= increment_rank(mask);
    return mask;
}

uint64_t get_black_front_file_mask(uint64_t piece)
{
    uint64_t mask = decrement_rank(piece);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    mask |= decrement_rank(mask);
    return mask;
}