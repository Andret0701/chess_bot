#pragma once
#include "board.h"
char* board_to_move(Board* from, Board* to);
bool move_valid(char *move);
bool can_move(Board* board, char* move);
Board apply_move(Board* board, char *move);