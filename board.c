#include "board.h"

bool pieces_equals(Pieces *a, Pieces *b)
{
    return a->bishops == b->bishops &&
           a->knights == b->knights &&
           a->pawns == b->pawns &&
           a->queens == b->queens &&
           a->rooks == b->rooks &&
           a->king == b->king;
}

bool board_equals(Board *a, Board *b)
{
    return pieces_equals(&a->white_pieces, &b->white_pieces) && pieces_equals(&a->black_pieces, &b->black_pieces) && a->en_passant == b->en_passant && a->castling_rights == b->castling_rights && a->side_to_move == b->side_to_move;
}