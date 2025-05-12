#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../../utils/board.h"
#include "../board_score.h"
#include "../../engine/board_state.h"

bool has_insufficient_material(Board *board);
Result get_result(BoardState *board_state, bool is_finished);
double score_board(BoardState *board_state);