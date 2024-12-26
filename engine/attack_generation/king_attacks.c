#include "attack_generation.h"
uint64_t generate_king_attacks(uint64_t kings)
{
    uint64_t attacks = 0;

    // Horizontal moves
    uint64_t east = (kings << 1) & 0xfefefefefefefefeULL;
    uint64_t west = (kings >> 1) & 0x7f7f7f7f7f7f7f7fULL;

    // Vertical moves
    uint64_t north = kings << 8;
    uint64_t south = kings >> 8;

    // Diagonal moves
    uint64_t northeast = (north << 1) & 0xfefefefefefefefeULL;
    uint64_t northwest = (north >> 1) & 0x7f7f7f7f7f7f7f7fULL;
    uint64_t southeast = (south << 1) & 0xfefefefefefefefeULL;
    uint64_t southwest = (south >> 1) & 0x7f7f7f7f7f7f7f7fULL;

    // Combine all possible moves
    attacks = east | west | north | south | northeast | northwest | southeast | southwest;

    return attacks;
}