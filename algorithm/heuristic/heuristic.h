#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "utils/board.h"
#include "algorithm/board_score.h"
#include "engine/board_state.h"

bool has_insufficient_material(Board *board);
Result get_result(BoardState *board_state, bool is_finished);
double get_game_phase(Board *board);
int32_t score_board(BoardState *board_state);