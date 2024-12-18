#pragma once

#include <stdint.h>
#include "board.h"
#include "board_stack.h"
#include "utils.h"

void init_board(BoardState *board_state);
void validate_white_move(BoardStack *stack);
void validate_black_move(BoardStack *stack);

static inline void generate_white_pawn_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
static inline void generate_black_pawn_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);