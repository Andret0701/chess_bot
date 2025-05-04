#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../utils/board.h"

#define FROM_MASK 0b0000000000111111
#define TO_MASK 0b0000111111000000
#define PROMOTION_MASK 0b0011000000000000
#define CAPTURE_MASK 0b0100000000000000
#define SPECIAL_MASK 0b1000000000000000 // Special is set to true if the move is a special move (castling, en passant or promotion)

#define PROMOTION_KNIGHT 0b00
#define PROMOTION_BISHOP 0b01
#define PROMOTION_ROOK 0b10
#define PROMOTION_QUEEN 0b11

uint8_t get_from_square(uint16_t move);
uint8_t get_to_square(uint16_t move);
uint8_t get_promotion_piece(uint16_t move);
bool is_capture_move(uint16_t move);
bool is_special_move(uint16_t move);

uint16_t set_from_square(uint16_t move, uint8_t from);
uint16_t set_to_square(uint16_t move, uint8_t to);
uint16_t set_promotion_piece(uint16_t move, uint8_t promotion);
uint16_t set_capture_move(uint16_t move, bool capture);
uint16_t set_special_move(uint16_t move, bool special);

uint16_t new_simple_encoded_move(uint8_t from, uint8_t to, bool is_capture);
uint16_t new_en_passant_encoded_move(uint8_t from, uint8_t to);
uint16_t new_castling_encoded_move(uint8_t from, uint8_t to);
uint16_t new_promotion_encoded_move(uint8_t from, uint8_t to, uint8_t promotion, bool is_capture);

bool encoded_move_equals(uint16_t move1, uint16_t move2);

uint16_t board_to_encoded_move(const Board *from, const Board *to);
uint16_t encode_move(const char *move, bool is_capture, bool is_special);
void print_move(uint16_t move);
