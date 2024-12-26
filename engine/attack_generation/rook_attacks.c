#include "attack_generation.h"
#include "rook_magic_numbers.h"
uint64_t generate_rook_attacks(uint64_t occupied, uint8_t i)
{
    MagicNumber magic_number = rook_magic_numbers[i];
    uint64_t magic_index = ((occupied & magic_number.mask) * magic_number.magic) >> magic_number.shift;
    return rook_magic_numbers_attack_tables[magic_index + magic_number.offset];
}