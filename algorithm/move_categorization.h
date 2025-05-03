#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../engine/board_state.h"

bool is_move_capture(BoardState *from, BoardState *to);
bool is_move_promotion(BoardState *from, BoardState *to);
bool is_move_threatening_promotion(BoardState *from, BoardState *to);
bool is_move_check(BoardState *to);