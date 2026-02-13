#pragma once

#include <stdint.h>
#include <time.h>
#include "../utils/board.h"
#include "../engine/board_stack.h"
#include "board_score.h"
#include "search_result.h"

int32_t quiescence(BoardState *board_state,
                   BoardStack *stack,
                   int32_t alpha,
                   int32_t beta,
                   int depth_in_quiescence, uint64_t *nodes_searched);
