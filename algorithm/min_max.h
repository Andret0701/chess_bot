#pragma once

#include <stdint.h>
#include <time.h>
#include "../utils/board.h"
#include "../engine/board_stack.h"
#include "board_score.h"

typedef struct
{
    BoardScore board_score;
    bool valid;
} SearchResult;

SearchResult quiesce(BoardState *board_state, BoardStack *stack, BoardScore alpha, BoardScore beta,
                     uint8_t depth, clock_t start, double seconds);
SearchResult min_max(Color bot_color, BoardState *board_state, BoardStack *stack, uint8_t max_depth, uint8_t depth, BoardScore alpha, BoardScore beta, clock_t start, double seconds);
void print_search_result(SearchResult search_result);