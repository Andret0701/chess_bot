#pragma once

#include "../../engine/board_state.h"
#include <stdint.h>
#include "feature.h"

typedef struct
{
    Feature bishop_my_side_attacks;
    Feature bishop_opponent_side_attacks;
    Feature rook_my_side_attacks;
    Feature rook_opponent_side_attacks;
    Feature queen_my_side_attacks;
    Feature queen_opponent_side_attacks;
} SquareControlFeatures;

SquareControlFeatures get_square_control_features(BoardState *board_state, double game_phase);