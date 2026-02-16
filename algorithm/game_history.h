#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../utils/board.h"

void reset_game_history();
void force_push_game_history(Board *from, Board *to);
void push_game_history(uint64_t hash);
void pop_game_history(uint64_t hash);
bool threefold_repetition();