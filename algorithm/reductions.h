#pragma once

#include <stdint.h>
#include "../engine/board_state.h"

void init_reductions();
uint8_t calculate_reduction(BoardState *current, BoardState *next, uint8_t depth, uint16_t move_number);