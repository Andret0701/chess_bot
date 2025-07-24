#include "attack_generation.h"

static inline uint64_t generate_queen_attacks(uint64_t occupied, uint64_t pieces)
{
    return generate_bishop_attacks(occupied, pieces) | generate_rook_attacks(occupied, pieces);
}

static inline uint64_t generate_queen_attack(uint64_t occupied, int index)
{
    return generate_bishop_attack(occupied, index) | generate_rook_attack(occupied, index);
}