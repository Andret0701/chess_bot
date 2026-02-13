#pragma once

#include "../utils/board.h"
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

typedef enum
{
    WON,
    LOST,
    STALEMATE,
    THREEFOLD_REPETITION,
    FIFTY_MOVE_RULE,
    INSUFFICIENT_MATERIAL,
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
bool is_draw(Result result);
BoardScore invert_score(BoardScore score);
const char *result_to_string(Result result);
void print_score(BoardScore score);

bool is_greater_score(BoardScore a, BoardScore b);
bool is_equal_score(BoardScore a, BoardScore b);
bool is_greater_equal_score(BoardScore a, BoardScore b);
bool is_less_score(BoardScore a, BoardScore b);
bool is_less_equal_score(BoardScore a, BoardScore b);

BoardScore max_score(BoardScore a, BoardScore b);
BoardScore min_score(BoardScore a, BoardScore b);
