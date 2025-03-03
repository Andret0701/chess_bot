#pragma once

#include "../../utils/board.h"

void validate_kings(Board *board);
void validate_pawns(Board *board);
void validate_castling_rights(Board *board);
void validate_no_duplicates(Board *board);
void validate_en_passant(Board *board);
void validate_check(Board *board);
void validate_board(Board *board);
void validate_castling(BoardState *from, BoardState *to);