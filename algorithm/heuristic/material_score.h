#pragma once

#include "utils/board.h"
#include <stdint.h>

int32_t get_material_score(Board *board, uint8_t middlegame_phase, uint8_t endgame_phase);