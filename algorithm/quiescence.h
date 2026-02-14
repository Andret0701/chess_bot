#pragma once

#include <stdint.h>
#include <time.h>
#include "../utils/board.h"
#include "../engine/board_stack.h"
#include "board_score.h"

int32_t quiescence(BoardState *board_state,
                   BoardStack *stack,
                   int32_t alpha,
                   int32_t beta,
                   uint8_t depth,
                   uint8_t depth_in_quiescence, uint64_t *nodes_searched);
