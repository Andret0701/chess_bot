#include "attack_generation.h"
#include "knight_attack_table.h"

uint64_t generate_knight_attacks(uint8_t i)
{
    return knight_attack_table[i];
}