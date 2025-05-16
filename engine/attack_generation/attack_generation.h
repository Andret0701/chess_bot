#pragma once

#include <stdint.h>
#include "../board_state.h"

static inline uint64_t generate_white_pawn_attacks(uint64_t pawns);
static inline uint64_t generate_black_pawn_attacks(uint64_t pawns);
static inline uint64_t generate_knight_attacks(uint64_t knights);
static inline uint64_t generate_bishop_attacks(uint64_t occupied, uint8_t i);
static inline uint64_t generate_rook_attacks(uint64_t occupied, uint8_t i);
static inline uint64_t generate_queen_attacks(uint64_t occupied, uint8_t i);
static inline uint64_t generate_king_attacks(uint64_t kings);

void generate_attacks(BoardState *board_state);

uint64_t generate_white_attacks(Board *board);
uint64_t generate_black_attacks(Board *board);