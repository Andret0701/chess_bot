#pragma once

#include <stdint.h>
#include <time.h>
#include "../utils/board.h"
#include "../engine/board_stack.h"
#include "board_score.h"
#include "search_result.h"

SearchResult negamax(BoardState *board_state, BoardStack *stack, uint8_t max_depth, uint8_t depth, BoardScore alpha, BoardScore beta, clock_t start, double seconds);