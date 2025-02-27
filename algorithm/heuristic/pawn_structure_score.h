#pragma once

#include "../../engine/board_state.h"
#include <stdint.h>

// Double pawn penalty: -20 for each extra pawn on lfile
// Isolated pawn penalty: -20 for each isolated pawn
// Backward pawn penalty: -10 for each backward pawn
// Passed pawn bonus: 10 for each passed pawn (maybe add bonus for how far it is)
// Pawn island penalty: -10 for each pawn island
// Pawn chain bonus: 10 for each pawn in a chain

int32_t get_pawn_structure_score(BoardState *board_state);