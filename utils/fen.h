#pragma once

#include <stdint.h>
#include "board.h"

#define STARTFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"

Board fen_to_board(const char *fen);
char *board_to_fen(const Board *board);