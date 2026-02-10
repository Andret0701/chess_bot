#pragma once

#include <stdint.h>
#include <time.h>
#include "../utils/board.h"
#include "../engine/board_stack.h"
#include "board_score.h"
#include "search_result.h"

SearchResult nega_scout(BoardState *board_state, BoardStack *stack, uint8_t max_depth, uint8_t depth, BoardScore alpha, BoardScore beta, bool use_max_time, clock_t start, double seconds, bool use_max_nodes, uint64_t *nodes_searched, uint64_t max_nodes, bool allow_null_move);