#include "move_categorization.h"

bool is_move_capture(BoardState *from, BoardState *to)
{
    if (from->board.side_to_move == WHITE)
        return from->black_pieces != to->black_pieces;
    return from->white_pieces != to->white_pieces;
}

bool is_move_promotion(BoardState *from, BoardState *to)
{
    if (from->board.side_to_move == WHITE)
        return __builtin_popcountll(from->board.white_pieces.pawns) != __builtin_popcountll(to->board.white_pieces.pawns);
    return __builtin_popcountll(from->board.black_pieces.pawns) != __builtin_popcountll(to->board.black_pieces.pawns);
}

bool is_move_check(BoardState *from, BoardState *to)
{
    if (from->board.side_to_move == WHITE)
        return to->black_check;
    return to->white_check;
}