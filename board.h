#pragma once
#include <stdint.h>

#include <stdint.h>
#include <stdbool.h>

#define WHITE_KINGSIDE_CASTLE (1 << 0)
#define WHITE_QUEENSIDE_CASTLE (1 << 1)
#define BLACK_KINGSIDE_CASTLE (1 << 2)
#define BLACK_QUEENSIDE_CASTLE (1 << 3)

typedef struct
{
    uint64_t pawns;
    uint64_t knights;
    uint64_t bishops;
    uint64_t rooks;
    uint64_t queens;
    uint64_t king;
    uint64_t total_pieces;
} Pieces;

typedef struct
{
    Pieces white_pieces;
    Pieces black_pieces;

    uint64_t occupied;

    Pieces white_attacks;
    Pieces black_attacks;

    uint64_t en_passant;
    uint8_t castling_rights;

    uint8_t side_to_move;
    bool white_check;
    bool black_check;
} Board;

bool pieces_equals(Pieces *a, Pieces *b);
bool board_equals(Board *a, Board *b);