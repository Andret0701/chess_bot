#pragma once

#include "../../utils/board.h"
#include <stdint.h>
#include "feature.h"

typedef struct
{
    Feature center_pawns;
    Feature bishop_pawns;
    Feature knight_pawns;
    Feature rook_pawns;
    Feature knights;
    Feature bishops;
    Feature first_rook;
    Feature additional_rooks;
    Feature first_queen;
    Feature additional_queens;
    Feature bishop_pair;
} MaterialFeatures;

MaterialFeatures get_material_features(Board *board, double game_phase);