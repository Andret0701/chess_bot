#pragma once

#include <stdint.h>
#include "../utils/board.h"

void init_zobrist_keys();
uint64_t hash_board(Board *board);