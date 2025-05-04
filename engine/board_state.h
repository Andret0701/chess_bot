#pragma once

#include "../utils/board.h"
#include "../algorithm/board_score.h"

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

    uint8_t has_castled;
    uint16_t move;         // Move that led to this position
    uint16_t mvvlva_score; // for sorting moves
} BoardState;

BoardState board_to_board_state(Board *board);
Result get_game_result(BoardState *board_state);