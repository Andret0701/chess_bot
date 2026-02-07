#include "attack_generation.h"

uint64_t generate_knight_attacks(uint64_t knights)
{
    uint64_t l1 = (knights >> 1) & 0x7f7f7f7f7f7f7f7fULL;
    uint64_t l2 = (knights >> 2) & 0x3f3f3f3f3f3f3f3fULL;
    uint64_t r1 = (knights << 1) & 0xfefefefefefefefeULL;
    uint64_t r2 = (knights << 2) & 0xfcfcfcfcfcfcfcfcULL;

    uint64_t h1 = l1 | r1;
    uint64_t h2 = l2 | r2;

    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}