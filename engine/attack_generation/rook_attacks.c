#include "attack_generation.h"
#include "rook_magic_numbers.h"
uint64_t generate_rook_attacks(uint64_t occupied, uint8_t i)
{
    uint64_t mask = rook_attack_masks[i];
    uint64_t magic = rook_magic_numbers[i];
    uint8_t shift = rook_magic_shifts[i];
    uint64_t magic_index = ((occupied & mask) * magic) >> shift;
    return rook_attack_tables[i][magic_index];
}