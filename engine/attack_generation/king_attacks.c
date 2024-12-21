#include "attack_generation.h"
#include "king_attack_table.h"
uint64_t generate_king_attacks(uint8_t i)
{
    return king_attack_table[i];
}