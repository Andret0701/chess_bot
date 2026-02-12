#include "attack_generation.h"
#include "utils/bitboard.h"

uint64_t generate_white_pawn_attacks(uint64_t pawns)
{
    uint64_t west_attacks = (pawns & (~FILE_A_MASK)) << 7; // Left diagonal
    uint64_t east_attacks = (pawns & (~FILE_H_MASK)) << 9; // Right diagonal
    return west_attacks | east_attacks;
}

uint64_t generate_black_pawn_attacks(uint64_t pawns)
{
    uint64_t east_attacks = (pawns & (~FILE_H_MASK)) >> 7; // Right diagonal
    uint64_t west_attacks = (pawns & (~FILE_A_MASK)) >> 9; // Left diagonal
    return east_attacks | west_attacks;
}
