#include "board.h"
#include <stdio.h>
#include "bitboard.h"
#include <string.h>

uint64_t pieces_to_bitboard(const Pieces *pieces)
{
    return pieces->pawns | pieces->knights | pieces->bishops | pieces->rooks | pieces->queens | pieces->king;
}

bool pieces_equals(const Pieces *a, const Pieces *b)
{
    return memcmp(a, b, sizeof(Pieces)) == 0;
}

bool board_equals(const Board *a, const Board *b)
{
    return memcmp(a, b, sizeof(Board)) == 0;
}

void copy_board(const Board *from, Board *to)
{
    memcpy(to, from, sizeof(Board));
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