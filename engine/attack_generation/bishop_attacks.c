#include "attack_generation.h"
#include "bishop_magic_numbers.h"
uint64_t generate_bishop_attacks(uint64_t occupied, uint8_t i)
{
    uint64_t mask = bishop_attack_masks[i];
    uint64_t magic = bishop_magic_numbers[i];
    uint8_t shift = bishop_magic_shifts[i];
    uint64_t magic_index = ((occupied & mask) * magic) >> shift;
    return bishop_attack_tables[i][magic_index];
}