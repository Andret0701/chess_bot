#pragma once

#include "../../engine/board_state.h"
#include <stdint.h>
#include "feature.h"

typedef struct
{
    Feature one_square_left;
    Feature two_squares_left;
    Feature three_squares_left;
    Feature four_squares_left;
    Feature five_squares_left;
    Feature six_squares_left;
} PassedPawnFeatures;

typedef struct
{
    Feature protected_pawns;
    Feature protected_by_protected_pawns;
} PawnChainFeatures;

typedef struct
{
    Feature double_pawns;
    Feature isolated_pawns;
    Feature backward_pawns;
    PassedPawnFeatures passed_pawns;
    PawnChainFeatures pawn_chains;
} PawnStructureFeatures;

PawnStructureFeatures get_pawn_structure_features(BoardState *board_state, double game_phase);