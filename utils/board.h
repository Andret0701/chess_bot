#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define WHITE_KINGSIDE_CASTLE (1 << 0)
#define WHITE_QUEENSIDE_CASTLE (1 << 1)
#define BLACK_KINGSIDE_CASTLE (1 << 2)
#define BLACK_QUEENSIDE_CASTLE (1 << 3)

typedef enum
{
    WHITE,
    BLACK
} Color;

typedef struct
{
    uint64_t pawns;
    uint64_t knights;
    uint64_t bishops;
    uint64_t rooks;
    uint64_t queens;
    uint64_t king;
} Pieces;

typedef struct
{
    Pieces white_pieces;
    Pieces black_pieces;
    uint64_t en_passant;
    uint8_t castling_rights;
    Color side_to_move;
} Board;

uint64_t pieces_to_bitboard(const Pieces *pieces);
bool pieces_equals(const Pieces *a, const Pieces *b);
bool board_equals(Board *a, Board *b);
void copy_board(Board *from, Board *to);
Board flip_board(Board *board);

void print_board_to_file(Board *board, FILE *file);
void print_board(Board *board);