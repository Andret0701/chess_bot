#pragma once

#include <stdint.h>
#include "../board_state.h"

uint64_t generate_white_pawn_attacks(uint8_t x, uint8_t y);
uint64_t generate_black_pawn_attacks(uint8_t x, uint8_t y);
uint64_t generate_knight_attacks(uint8_t x, uint8_t y);
uint64_t generate_bishop_attacks(BoardState *board_state, uint8_t x, uint8_t y);
uint64_t generate_rook_attacks(BoardState *board_state, uint8_t x, uint8_t y);
uint64_t generate_queen_attacks(BoardState *board_state, uint8_t x, uint8_t y);
uint64_t generate_king_attacks(uint8_t x, uint8_t y);

void generate_attacks(BoardState *board_state);