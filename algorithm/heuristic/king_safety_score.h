#pragma once

#include "../../engine/board_state.h"
#include <stdint.h>

double get_king_safety_score(BoardState *board_state, double game_phase);