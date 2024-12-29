#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../board.h"

void reset_game_history();
void push_game_history(Board board);
void pop_game_history();
void print_game_history();

bool threefold_repetition();
bool has_repeated_position();