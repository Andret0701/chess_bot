#pragma once

#include "../board.h"
#include <stdint.h>

typedef enum
{
    WHITE_WON,
    BLACK_WON,
    DRAW,
    UNKNOWN
} Result;

typedef struct
{
    int32_t score;
    Result result;
    uint8_t depth;
} BoardScore;

bool has_won(Result result, Color color);
bool has_lost(Result result, Color color);
BoardScore get_worst_score(Color color);

bool is_better_score(BoardScore a, BoardScore b, Color color);
bool is_better_equal(BoardScore a, BoardScore b, Color color);
BoardScore max_score(BoardScore a, BoardScore b, Color color);

void print_score(BoardScore score);