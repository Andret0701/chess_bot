#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../utils/board.h"

void reset_game_history();
void push_game_history(Board board);
void pop_game_history();
void print_game_history();

bool threefold_repetition();
bool has_50_move_rule_occurred();
uint8_t get_50_move_count();

uint8_t get_full_move_count();
uint8_t get_half_move_count();