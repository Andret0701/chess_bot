#include "attack_generation.h"

uint64_t generate_queen_attacks(uint64_t occupied, uint8_t i)
{
    return generate_bishop_attacks(occupied, i) | generate_rook_attacks(occupied, i);
}