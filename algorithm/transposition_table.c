#include "transposition_table.h"
#include <time.h>
#include <stdio.h>

#define NUM_TT_ENTRIES (1 << 25)
#define TT_MASK (NUM_TT_ENTRIES - 1)

uint8_t current_generation = 0;

// At the start of a new search:
void TT_clear_generation()
{
    current_generation++; // Global counter
}

TT_Entry transposition_table[NUM_TT_ENTRIES] = {0};
void TT_prefetch(uint64_t hash)
{
    TT_Entry *tt = &transposition_table[hash & TT_MASK];
    __builtin_prefetch(tt, 0, 3); // Prefetch for read with temporal locality
}

bool TT_lookup(uint64_t hash, TT_Entry *entry)
{
    TT_Entry *tt = &transposition_table[hash & TT_MASK];
    if (tt->hash != hash)
        return false;

    *entry = *tt;
    tt->generation = current_generation;
    return true;
}

void TT_store(uint64_t hash, uint8_t depth, double score,
              Result result, TT_Entry_Type type, uint16_t move)
{
    TT_Entry *entry = &transposition_table[hash & TT_MASK];
    if (entry->hash == hash && entry->depth > depth && entry->type == EXACT)
    {
        // If the entry is already present and has a greater depth, we do not overwrite it.
        entry->generation = current_generation;
        return;
    }

    // Replacement scheme: prefer higher depth or exact scores
    if (entry->generation != current_generation ||
        depth > entry->depth ||
        (depth == entry->depth && (type == EXACT || entry->type != EXACT)))
    {
        *entry = (TT_Entry){hash, depth, score, result, type, move, current_generation};
    }
}

void TT_log_stats()
{
    size_t used_entries = 0;
    size_t exact_count = 0;
    size_t lowerbound_count = 0;
    size_t total_depth = 0;

    for (size_t i = 0; i < NUM_TT_ENTRIES; i++)
    {
        TT_Entry *entry = &transposition_table[i];
        if (entry->generation == current_generation)
        {
            used_entries++;
            total_depth += entry->depth;

            if (entry->type == EXACT)
                exact_count++;
            else if (entry->type == LOWERBOUND)
                lowerbound_count++;
        }
    }

    printf("==== Transposition Table Stats ====\n");
    printf("Generation: %u\n", current_generation);
    printf("Used entries: %llu / %llu (%.2f%%)\n", (unsigned long long)used_entries, (unsigned long long)NUM_TT_ENTRIES, 100.0 * used_entries / NUM_TT_ENTRIES);
    printf("EXACT entries:     %llu\n", (unsigned long long)exact_count);
    printf("LOWERBOUND entries: %llu\n", (unsigned long long)lowerbound_count);
    if (used_entries > 0)
        printf("Average depth stored: %.2f\n", (double)total_depth / used_entries);
    else
        printf("Average depth stored: N/A\n");
    printf("===================================\n");
}
