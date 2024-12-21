#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../board.h"
#include "board_score.h"
#include "../engine/board_state.h"

int piece_values[6] = {100, 320, 330, 500, 900}; // Pawn, Knight, Bishop, Rook, Queen
BoardScore score_board(BoardState *board_state, uint8_t depth, bool is_finished);