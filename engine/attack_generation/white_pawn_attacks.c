#include "attack_generation.h"
#include "white_pawn_attack_tables.h"
uint64_t generate_white_pawn_attacks(uint8_t i)
{
    return white_pawn_attack_table[i];
}