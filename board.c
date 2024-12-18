#include "board.h"
#include <stdio.h>
#include "utils.h"

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

void copy_board(Board *from, Board *to)
{
    to->white_pieces = from->white_pieces;
    to->black_pieces = from->black_pieces;
    to->en_passant = from->en_passant;
    to->castling_rights = from->castling_rights;
}

void print_board(Board *board)
{
    for (int y = 7; y >= 0; y--)
    {
        for (int x = 0; x < 8; x++)
        {
            uint64_t position = position_to_u64(x, y);
            if (board->white_pieces.pawns & position)
                printf("P ");
            else if (board->white_pieces.knights & position)
                printf("N ");
            else if (board->white_pieces.bishops & position)
                printf("B ");
            else if (board->white_pieces.rooks & position)
                printf("R ");
            else if (board->white_pieces.queens & position)
                printf("Q ");
            else if (board->white_pieces.king & position)
                printf("K ");
            else if (board->black_pieces.pawns & position)
                printf("p ");
            else if (board->black_pieces.knights & position)
                printf("n ");
            else if (board->black_pieces.bishops & position)
                printf("b ");
            else if (board->black_pieces.rooks & position)
                printf("r ");
            else if (board->black_pieces.queens & position)
                printf("q ");
            else if (board->black_pieces.king & position)
                printf("k ");
            else
                printf(". ");
        }
        printf("\n");
    }
}