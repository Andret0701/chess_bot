#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../utils/board.h"
#include "../engine/board_stack.h"

void reset_game_history();
void force_push_game_history(Board *from, Board *to);
void push_game_history(uint64_t hash);
void pop_game_history(uint64_t hash);
bool threefold_repetition();
bool threefold_repetition_possible();
uint16_t get_move_count();

bool check_opponent_can_threefold(BoardState *board_state, BoardStack *stack);