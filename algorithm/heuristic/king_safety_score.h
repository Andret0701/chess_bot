#pragma once

#include "../../engine/board_state.h"
#include <stdint.h>
#include "feature.h"

typedef struct
{
    Feature can_castle_both_sides;
    Feature can_castle_kingside;
    Feature can_castle_queenside;
    Feature has_castled_kingside;
    Feature has_castled_queenside;
} CastlingFeatures;

typedef struct
{
    Feature front_pawn;
    Feature ahead_pawn;
    Feature left_pawn;
    Feature right_pawn;
} PawnShelterFeatures;

typedef struct
{
    CastlingFeatures castling;
    PawnShelterFeatures pawn_shelter;
    Feature attacking_king_squares;
} KingSafetyFeatures;

KingSafetyFeatures get_king_safety_features(BoardState *board_state, double game_phase);