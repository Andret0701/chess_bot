#pragma once

#include "engine/board_state.h"
#include <stdint.h>

int32_t get_king_safety_score(BoardState *board_state, uint8_t middlegame_phase, uint8_t endgame_phase);