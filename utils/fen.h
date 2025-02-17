#pragma once

#include <stdint.h>
#include "board.h"

Board fen_to_board(const char *fen);
char *board_to_fen(const Board *board);