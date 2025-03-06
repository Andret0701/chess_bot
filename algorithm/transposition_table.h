#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "../utils/board.h"
#include "board_score.h"

typedef enum
{
    EXACT,
    LOWERBOUND,
    UPPERBOUND
} TT_Entry_Type;

typedef struct
{
    uint64_t hash;      // Zobrist hash of the position.
    uint8_t depth;      // Remaining depth at which this evaluation was computed.
    int32_t score;      // The evaluation score.
    Result result;      // The result of the evaluation.
    TT_Entry_Type type; // The type of the evaluation.
    uint8_t generation; // The generation of the entry.
} TT_Entry;

void TT_clear_generation();
void init_transposition_table();
static bool TT_lookup(uint64_t hash, TT_Entry *entry, uint8_t depth);
static void TT_store(uint64_t hash, uint8_t depth, int32_t score, Result result, TT_Entry_Type type);
void TT_log_stats();