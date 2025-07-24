#include "attack_generation.h"
#include "bishop_magic_numbers.h"

static inline uint64_t generate_bishop_attacks(uint64_t occupied, uint64_t pieces)
{
    uint64_t attacks = 0;
    while (pieces)
    {
        int index = __builtin_ctzll(pieces);
        pieces &= pieces - 1;
        attacks |= generate_bishop_attack(occupied, index);
    }
    return attacks;
}

static inline uint64_t generate_bishop_attack(uint64_t occupied, int index)
{
    const MagicNumber *magic_number = &bishop_magic_numbers[index];
    uint64_t magic_index = ((occupied & magic_number->mask) * magic_number->magic) >> magic_number->shift;
    return bishop_magic_numbers_attack_tables[magic_index + magic_number->offset];
}