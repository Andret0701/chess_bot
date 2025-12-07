#pragma once

#include "../../utils/board.h"
#include <stdint.h>
#include "feature.h"

double get_position_score(Board *board, double game_phase);

typedef struct
{
    Feature *pawns;
    Feature *knights;
    Feature *bishops;
    Feature *rooks;
    Feature *queens;
    Feature *king;
} PositionFeatures;

PositionFeatures get_position_features(Board *board, double game_phase);
void free_position_features(PositionFeatures position_features);