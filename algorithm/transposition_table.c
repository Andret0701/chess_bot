#include "transposition_table.h"
#include <time.h>

#define NUM_TT_ENTRIES (1 << 24)
#define TT_MASK (NUM_TT_ENTRIES - 1)

static uint64_t rng_state;

uint8_t current_generation = 0;

// At the start of a new search:
void TT_clear_generation()
{
    current_generation++; // Global counter
}

static TT_Entry transposition_table[NUM_TT_ENTRIES] = {0};
static bool TT_lookup(uint64_t hash, TT_Entry *entry)
{
    size_t index = hash & TT_MASK;
    TT_Entry *tt = &transposition_table[index];
    __builtin_prefetch(tt, 0, 1);
    if (__builtin_expect(tt->hash != hash, 0))
        return false;
    *entry = *tt;
    tt->generation = current_generation;
    return true;
}

void TT_store(uint64_t hash, uint8_t depth, double score,
              Result result, TT_Entry_Type type, uint16_t move)
{
    size_t index = hash & TT_MASK;
    TT_Entry *entry = &transposition_table[index];

    if (entry->generation != current_generation)
    {
        *entry = (TT_Entry){hash, depth, score, result, type, move, current_generation};
        return;
    }

    if (entry->depth <= depth)
    {
        *entry = (TT_Entry){hash, depth, score, result, type, move, current_generation};
        return;
    }

    if (entry->type != EXACT && type == EXACT)
    {
        *entry = (TT_Entry){hash, depth, score, result, type, move, current_generation};
        return;
    }
}