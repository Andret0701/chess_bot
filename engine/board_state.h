#pragma once

#include "../board.h"

typedef struct
{
    Board board;

    uint64_t white_pieces;
    uint64_t black_pieces;
    uint64_t occupied;

    Pieces white_attacks;
    Pieces black_attacks;

    uint64_t white_attack;
    uint64_t black_attack;

    bool white_check;
    bool black_check;
} BoardState;
