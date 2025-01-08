#pragma once

#include "../../board.h"

typedef enum
{
    OPENING,
    MIDGAME,
    ENDGAME
} GamePhase;

GamePhase get_game_phase(Board *board);