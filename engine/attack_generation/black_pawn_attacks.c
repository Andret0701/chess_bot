#include "attack_generation.h"

#define NOT_A_FILE 0xfefefefefefefefe
#define NOT_H_FILE 0x7f7f7f7f7f7f7f7f

uint64_t generate_black_pawn_attacks(uint64_t pawns)
{
    uint64_t east_attacks = (pawns & NOT_H_FILE) >> 7;
    uint64_t west_attacks = (pawns & NOT_A_FILE) >> 9;
    return east_attacks | west_attacks;
}
