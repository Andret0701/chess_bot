#include "attack_generation.h"

#define NOT_A_FILE 0xfefefefefefefefeULL // Mask to clear the A-file
#define NOT_H_FILE 0x7f7f7f7f7f7f7f7fULL // Mask to clear the H-file

uint64_t generate_white_pawn_attacks(uint64_t pawns)
{
    uint64_t west_attacks = (pawns & NOT_A_FILE) << 7; // Left diagonal
    uint64_t east_attacks = (pawns & NOT_H_FILE) << 9; // Right diagonal
    return west_attacks | east_attacks;
}

uint64_t generate_black_pawn_attacks(uint64_t pawns)
{
    uint64_t east_attacks = (pawns & NOT_H_FILE) >> 7; // Right diagonal
    uint64_t west_attacks = (pawns & NOT_A_FILE) >> 9; // Left diagonal
    return east_attacks | west_attacks;
}
