#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../../engine/board_state.h"

bool init_endgames();
void free_endgames();

bool is_known_endgame(BoardState *board_state);
BoardScore score_endgame(BoardState *board_state);