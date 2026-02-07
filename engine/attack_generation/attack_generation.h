#pragma once

#include <stdint.h>
#include "../board_state.h"

uint64_t generate_white_pawn_attacks(uint64_t pawns);
uint64_t generate_black_pawn_attacks(uint64_t pawns);
uint64_t generate_knight_attacks(uint64_t knights);
uint64_t generate_bishop_attacks(uint64_t occupied, uint64_t pieces);
uint64_t generate_rook_attacks(uint64_t occupied, uint64_t pieces);
uint64_t generate_queen_attacks(uint64_t occupied, uint64_t pieces);
uint64_t generate_king_attacks(uint64_t kings);

uint64_t generate_bishop_attack(uint64_t occupied, int index);
uint64_t generate_rook_attack(uint64_t occupied, int index);
uint64_t generate_queen_attack(uint64_t occupied, int index);

void generate_attacks(BoardState *board_state);
void generate_white_attacks(const BoardState *from, BoardState *to);
void generate_black_attacks(const BoardState *from, BoardState *to);

uint64_t generate_white_attacks_mask(Board *board);
uint64_t generate_black_attacks_mask(Board *board);