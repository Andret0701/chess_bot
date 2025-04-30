#pragma once

#include "../utils/board.h"
#include <stdint.h>

typedef enum
{
    WON,
    LOST,
    DRAW,
    UNKNOWN
} Result;

typedef struct
{
    int32_t score;
    Result result;
    uint8_t depth;
} BoardScore;

static const BoardScore BEST_SCORE = {INT32_MAX, WON, 0};
static const BoardScore WORST_SCORE = {INT32_MIN, LOST, 0};
BoardScore invert_score(BoardScore score);
void print_score(BoardScore score);

bool is_greater_score(BoardScore a, BoardScore b);
bool is_equal_score(BoardScore a, BoardScore b);
bool is_greater_equal_score(BoardScore a, BoardScore b);

BoardScore max_score(BoardScore a, BoardScore b);
BoardScore min_score(BoardScore a, BoardScore b);
