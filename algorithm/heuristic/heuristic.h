#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../../utils/board.h"
#include "../board_score.h"
#include "../../engine/board_state.h"

bool has_insufficient_material(Board *board);
Result get_result(BoardState *board_state, bool is_finished);
BoardScore score_board(BoardState *board_state, uint8_t depth, bool is_finished);