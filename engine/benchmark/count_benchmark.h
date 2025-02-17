#pragma once

#include <stdint.h>
#include "../../utils/board.h"
#include "../board_stack.h"

uint64_t count_recursive(BoardState *board_state, uint8_t depth, BoardStack *stack);
void run_count_benchmark();