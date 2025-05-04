#include "board.h"
#include <stdio.h>
#include "bitboard.h"

uint64_t pieces_to_bitboard(const Pieces *pieces)
{
    return pieces->pawns | pieces->knights | pieces->bishops | pieces->rooks | pieces->queens | pieces->king;
}

bool pieces_equals(const Pieces *a, const Pieces *b)
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
    to->side_to_move = from->side_to_move;
}

Board flip_board(Board *board)
{
    Board flipped = {0};
    flipped.white_pieces.pawns = flip_bitboard(board->black_pieces.pawns);
    flipped.white_pieces.knights = flip_bitboard(board->black_pieces.knights);
    flipped.white_pieces.bishops = flip_bitboard(board->black_pieces.bishops);
    flipped.white_pieces.rooks = flip_bitboard(board->black_pieces.rooks);
    flipped.white_pieces.queens = flip_bitboard(board->black_pieces.queens);
    flipped.white_pieces.king = flip_bitboard(board->black_pieces.king);

    flipped.black_pieces.pawns = flip_bitboard(board->white_pieces.pawns);
    flipped.black_pieces.knights = flip_bitboard(board->white_pieces.knights);
    flipped.black_pieces.bishops = flip_bitboard(board->white_pieces.bishops);
    flipped.black_pieces.rooks = flip_bitboard(board->white_pieces.rooks);
    flipped.black_pieces.queens = flip_bitboard(board->white_pieces.queens);
    flipped.black_pieces.king = flip_bitboard(board->white_pieces.king);

    flipped.en_passant = flip_bitboard(board->en_passant);

    flipped.castling_rights = 0;
    if (board->castling_rights & WHITE_KINGSIDE_CASTLE)
        flipped.castling_rights |= BLACK_KINGSIDE_CASTLE;
    if (board->castling_rights & WHITE_QUEENSIDE_CASTLE)
        flipped.castling_rights |= BLACK_QUEENSIDE_CASTLE;
    if (board->castling_rights & BLACK_KINGSIDE_CASTLE)
        flipped.castling_rights |= WHITE_KINGSIDE_CASTLE;
    if (board->castling_rights & BLACK_QUEENSIDE_CASTLE)
        flipped.castling_rights |= WHITE_QUEENSIDE_CASTLE;

    flipped.side_to_move = board->side_to_move == WHITE ? BLACK : WHITE;
    return flipped;
}

void print_board_to_file(Board *board, FILE *file)
{
    for (int y = 7; y >= 0; y--)
    {
        for (int x = 0; x < 8; x++)
        {
            uint64_t position = position_to_bitboard(x, y);
            if (board->white_pieces.pawns & position)
                fprintf(file, "P");
            if (board->white_pieces.knights & position)
                fprintf(file, "N");
            if (board->white_pieces.bishops & position)
                fprintf(file, "B");
            if (board->white_pieces.rooks & position)
                fprintf(file, "R");
            if (board->white_pieces.queens & position)
                fprintf(file, "Q");
            if (board->white_pieces.king & position)
                fprintf(file, "K");
            if (board->black_pieces.pawns & position)
                fprintf(file, "p");
            if (board->black_pieces.knights & position)
                fprintf(file, "n");
            if (board->black_pieces.bishops & position)
                fprintf(file, "b");
            if (board->black_pieces.rooks & position)
                fprintf(file, "r");
            if (board->black_pieces.queens & position)
                fprintf(file, "q");
            if (board->black_pieces.king & position)
                fprintf(file, "k");
            if (!((board->white_pieces.pawns | board->white_pieces.knights | board->white_pieces.bishops | board->white_pieces.rooks | board->white_pieces.queens | board->white_pieces.king | board->black_pieces.pawns | board->black_pieces.knights | board->black_pieces.bishops | board->black_pieces.rooks | board->black_pieces.queens | board->black_pieces.king) & position))
                fprintf(file, ".");
            fprintf(file, " ");
        }
        fprintf(file, "\n");
    }
}

void print_board(Board *board)
{
    print_board_to_file(board, stdout);
}