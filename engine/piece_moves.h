#pragma once

#include <stdint.h>
#include "../utils/board.h"
#include "board_stack.h"
#include "../utils/utils.h"
#include "../utils/bitboard.h"
#include "attack_generation/attack_generation.h"
#include "encoded_move.h"

void init_board(BoardState *board_state);
void validate_white_move(BoardStack *stack);
void validate_black_move(BoardStack *stack);

void remove_white_piece(BoardState *board_state, uint8_t x, uint8_t y);
void remove_black_piece(BoardState *board_state, uint8_t x, uint8_t y);

bool is_white_piece(BoardState *board_state, uint8_t x, uint8_t y);
bool is_black_piece(BoardState *board_state, uint8_t x, uint8_t y);

void generate_moves(BoardState *board_state, BoardStack *stack);
void generate_captures(BoardState *board_state, BoardStack *stack);
bool is_finished(BoardState *board_state);

void generate_white_pawn_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_pawn_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_white_knight_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_knight_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_white_bishop_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_bishop_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_white_rook_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_rook_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_white_queen_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_queen_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_white_king_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_king_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);

void generate_white_pawn_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_pawn_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_white_knight_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_knight_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_white_bishop_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_bishop_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_white_rook_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_rook_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_white_queen_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_queen_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_white_king_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);
void generate_black_king_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack);

bool white_pawn_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool black_pawn_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool white_knight_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool black_knight_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool white_bishop_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool black_bishop_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool white_rook_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool black_rook_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool white_queen_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool black_queen_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool white_king_can_move(BoardState *board_state, uint8_t x, uint8_t y);
bool black_king_can_move(BoardState *board_state, uint8_t x, uint8_t y);
