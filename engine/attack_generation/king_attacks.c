#include "attack_generation.h"
static inline uint64_t generate_king_attacks(uint64_t kings)
{
    uint64_t attacks = 0;
    // Vertical moves
    uint64_t north = kings << 8;
    uint64_t south = kings >> 8;
    attacks |= north | south;
    attacks |= (attacks << 1) & 0xfefefefefefefefeULL;
    attacks |= ((attacks | kings) >> 1) & 0x7f7f7f7f7f7f7f7fULL;
    attacks |= (kings << 1) & 0xfefefefefefefefeULL;

    return attacks;
}