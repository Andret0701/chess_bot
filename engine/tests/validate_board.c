#include "validate_board.h"
#include "../../utils/utils.h"
#include "../board_state.h"
#include <stdio.h>
#include <stdarg.h>
#include "../../utils/board.h"
#include "../piece_moves.h"
#include "../../utils/bitboard.h"

void errorf(Board *board, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    print_board(board);
    printf("\n");
}

void validate_kings(Board *board)
{
    if (popcountll(board->white_pieces.king) > 1)
        errorf(board, "White has %d kings\n", popcountll(board->white_pieces.king));

    if (popcountll(board->black_pieces.king) > 1)
        errorf(board, "Black has %d kings\n", popcountll(board->black_pieces.king));

    if (popcountll(board->white_pieces.king) == 0)
        errorf(board, "White has no king\n");

    if (popcountll(board->black_pieces.king) == 0)
        errorf(board, "Black has no king\n");
}

void validate_pawns(Board *board)
{
    if (board->white_pieces.pawns & RANK_1_MASK)
        errorf(board, "White pawn on last rank\n");

    if (board->black_pieces.pawns & RANK_8_MASK)
        errorf(board, "Black pawn on last rank\n");

    if (board->white_pieces.pawns & RANK_8_MASK)
        errorf(board, "White pawn on first rank\n");

    if (board->black_pieces.pawns & RANK_1_MASK)
        errorf(board, "Black pawn on first rank\n");
}

void validate_castling_rights(Board *board)
{
    if (board->castling_rights & WHITE_KINGSIDE_CASTLE)
    {
        if (!(board->white_pieces.rooks & position_to_bitboard(7, 0)))
            errorf(board, "White has kingside castling rights but no rook on h1\n");

        if (!(board->white_pieces.king & position_to_bitboard(4, 0)))
            errorf(board, "White has kingside castling rights but no king on e1\n");
    }

    if (board->castling_rights & WHITE_QUEENSIDE_CASTLE)
    {
        if (!(board->white_pieces.rooks & position_to_bitboard(0, 0)))
            errorf(board, "White has queenside castling rights but no rook on a1\n");

        if (!(board->white_pieces.king & position_to_bitboard(4, 0)))
            errorf(board, "White has queenside castling rights but no king on e1\n");
    }

    if (board->castling_rights & BLACK_KINGSIDE_CASTLE)
    {
        if (!(board->black_pieces.rooks & position_to_bitboard(7, 7)))
            errorf(board, "Black has kingside castling rights but no rook on h8\n");

        if (!(board->black_pieces.king & position_to_bitboard(4, 7)))
            errorf(board, "Black has kingside castling rights but no king on e8\n");
    }

    if (board->castling_rights & BLACK_QUEENSIDE_CASTLE)
    {
        if (!(board->black_pieces.rooks & position_to_bitboard(0, 7)))
            errorf(board, "Black has queenside castling rights but no rook on a8\n");

        if (!(board->black_pieces.king & position_to_bitboard(4, 7)))
            errorf(board, "Black has queenside castling rights but no king on e8\n");
    }
}

void validate_no_duplicates(Board *board)
{
    uint64_t bitboards[] = {board->white_pieces.pawns, board->white_pieces.knights, board->white_pieces.bishops, board->white_pieces.rooks, board->white_pieces.queens, board->white_pieces.king, board->black_pieces.pawns, board->black_pieces.knights, board->black_pieces.bishops, board->black_pieces.rooks, board->black_pieces.queens, board->black_pieces.king};
    const char *piece_names[] = {"white pawn", "white knight", "white bishop", "white rook", "white queen", "white king", "black pawn", "black knight", "black bishop", "black rook", "black queen", "black king"};
    const char *tile_names[] = {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};
    for (int i = 0; i < 12; i++)
    {
        for (int j = i + 1; j < 12; j++)
        {
            for (int k = 0; k < 64; k++)
            {

                if (bitboards[i] & bitboards[j] & (1ULL << k))
                    errorf(board, "There is both a %s and a %s on tile %s\n", piece_names[i], piece_names[j], tile_names[k]);
            }
        }
    }
}

void validate_en_passant(Board *board)
{
    if (board->en_passant)
    {
        if (board->side_to_move == WHITE)
        {
            if (!(board->black_pieces.pawns & (board->en_passant >> 8)))
                errorf(board, "No black pawn next to en passant square\n");
        }
        else
        {
            if (!(board->white_pieces.pawns & (board->en_passant << 8)))
                errorf(board, "No white pawn next to en passant square\n");
        }
    }
}

void validate_check(Board *board)
{
    BoardState board_state = {0};
    copy_board(board, &board_state.board);
    init_board(&board_state);

    if (board->side_to_move == WHITE)
    {
        if (board_state.black_check)
            errorf(board, "White is in check\n");
    }
    else
    {
        if (board_state.white_check)
            errorf(board, "Black is in check\n");
    }
}

void validate_board(Board *board)
{
    validate_kings(board);
    validate_pawns(board);
    validate_castling_rights(board);
    validate_no_duplicates(board);
    validate_en_passant(board);
    validate_check(board);
}

void validate_castling(BoardState *from, BoardState *to)
{
    if (from->has_castled != 0 && to->has_castled == 0)
        errorf(&to->board, "Has castled values were lost\n");

    uint8_t from_white_rights = from->board.castling_rights & (WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE);
    uint8_t to_white_rights = to->board.castling_rights & (WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE);
    uint8_t from_black_rights = from->board.castling_rights & (BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE);
    uint8_t to_black_rights = to->board.castling_rights & (BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE);

    uint8_t from_white_castled = from->has_castled & (WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE);
    uint8_t to_white_castled = to->has_castled & (WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE);
    uint8_t from_black_castled = from->has_castled & (BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE);
    uint8_t to_black_castled = to->has_castled & (BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE);

    if (from_white_castled == 0 && to_white_castled != 0)
    {
        if (to_white_rights != 0)
            errorf(&to->board, "White has castled but still has castling rights\n");

        if (from_white_rights == 0)
            errorf(&to->board, "White has castled but had no castling rights\n");
    }

    if (from_black_castled == 0 && to_black_castled != 0)
    {
        if (to_black_rights != 0)
            errorf(&to->board, "Black has castled but still has castling rights\n");

        if (from_black_rights == 0)
            errorf(&to->board, "Black has castled but had no castling rights\n");
    }

    if (from_white_rights == to_white_rights)
    {
        if (from_white_castled != to_white_castled)
            errorf(&to->board, "Rights and castled values for white do not match\n");
    }

    if (from_black_rights == to_black_rights)
    {
        if (from_black_castled != to_black_castled)
            errorf(&to->board, "Rights and castled values for black do not match\n");
    }
}