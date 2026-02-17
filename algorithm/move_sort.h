#pragma once

#include <stdint.h>
#include "../engine/board_stack.h"

void clear_killer_moves();
void add_killer_move(uint16_t move, uint8_t depth);
uint16_t get_mvvlva(BoardState *from, BoardState *to);
void sort_moves(BoardState *from, BoardStack *stack, uint16_t base, uint16_t tt_move);