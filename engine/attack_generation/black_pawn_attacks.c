#include "attack_generation.h"
#include "black_pawn_attack_tables.h"
uint64_t generate_black_pawn_attacks(uint8_t i)
{
    return black_pawn_attack_table[i];
}