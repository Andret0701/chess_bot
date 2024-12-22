#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../board.h"
#include "board_score.h"
#include "../engine/board_state.h"

BoardScore score_board(BoardState *board_state, uint8_t depth, bool is_finished);