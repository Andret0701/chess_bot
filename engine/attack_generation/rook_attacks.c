#include "attack_generation.h"
#include "rook_magic_numbers.h"

uint64_t generate_rook_attacks(uint64_t occupied, uint64_t pieces)
{
    uint64_t attacks = 0;
    while (pieces)
    {
        int index = __builtin_ctzll(pieces);
        pieces &= pieces - 1;
        attacks |= generate_rook_attack(occupied, index);
    }
    return attacks;
}

uint64_t generate_rook_attack(uint64_t occupied, int index)
{
    const MagicNumber *magic_number = &rook_magic_numbers[index];
    uint64_t magic_index = ((occupied & magic_number->mask) * magic_number->magic) >> magic_number->shift;
    return rook_magic_numbers_attack_tables[magic_index + magic_number->offset];
}