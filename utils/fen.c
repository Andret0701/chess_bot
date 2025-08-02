#include "../engine/piece_moves.h"
#include "bitboard.h"

Board fen_to_board(const char *fen)
{
    Board board = {0};

    int rank = 7;
    int file = 0;
    int i = 0;

    // Parse board pieces
    while (fen[i] && fen[i] != ' ')
    {
        switch (fen[i])
        {
        case 'P':
            board.white_pieces.pawns = set_bit(board.white_pieces.pawns, file, rank);
            file++;
            break;
        case 'N':
            board.white_pieces.knights = set_bit(board.white_pieces.knights, file, rank);
            file++;
            break;
        case 'B':
            board.white_pieces.bishops = set_bit(board.white_pieces.bishops, file, rank);
            file++;
            break;
        case 'R':
            board.white_pieces.rooks = set_bit(board.white_pieces.rooks, file, rank);
            file++;
            break;
        case 'Q':
            board.white_pieces.queens = set_bit(board.white_pieces.queens, file, rank);
            file++;
            break;
        case 'K':
            board.white_pieces.king = set_bit(board.white_pieces.king, file, rank);
            file++;
            break;
        case 'p':
            board.black_pieces.pawns = set_bit(board.black_pieces.pawns, file, rank);
            file++;
            break;
        case 'n':
            board.black_pieces.knights = set_bit(board.black_pieces.knights, file, rank);
            file++;
            break;
        case 'b':
            board.black_pieces.bishops = set_bit(board.black_pieces.bishops, file, rank);
            file++;
            break;
        case 'r':
            board.black_pieces.rooks = set_bit(board.black_pieces.rooks, file, rank);
            file++;
            break;
        case 'q':
            board.black_pieces.queens = set_bit(board.black_pieces.queens, file, rank);
            file++;
            break;
        case 'k':
            board.black_pieces.king = set_bit(board.black_pieces.king, file, rank);
            file++;
            break;
            file++;
            break;
        case '/':
            rank--;
            file = 0;
            break;
        default:
            file += fen[i] - '0';
            break;
        }
        i++;
    }

    // Skip space
    i++;

    // Parse side to move
    if (fen[i] == 'w')
    {
        board.side_to_move = WHITE;
    }
    else
    {
        board.side_to_move = BLACK;
    }

    // Skip space
    i += 2;

    // Parse castling rights
    while (fen[i] && fen[i] != ' ')
    {
        switch (fen[i])
        {
        case 'K':
            board.castling_rights |= WHITE_KINGSIDE_CASTLE;
            break;
        case 'Q':
            board.castling_rights |= WHITE_QUEENSIDE_CASTLE;
            break;
        case 'k':
            board.castling_rights |= BLACK_KINGSIDE_CASTLE;
            break;
        case 'q':
            board.castling_rights |= BLACK_QUEENSIDE_CASTLE;
            break;
        }
        i++;
    }

    // Skip space
    i++;

    // Parse en passant square
    if (fen[i] != '-')
    {
        int file = fen[i] - 'a';
        int rank = fen[i + 1] - '1';
        board.en_passant = 1ULL << (rank * 8 + file);
    }

    return board;
}
char *board_to_fen(const Board *board)
{
    static char fen[100];
    int index = 0;
    int empty_count;

    // Encode board pieces
    for (int rank = 7; rank >= 0; rank--)
    {
        empty_count = 0;
        for (int file = 0; file < 8; file++)
        {
            int position = rank * 8 + file;

            if (board->white_pieces.pawns & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'P';
            }
            else if (board->white_pieces.knights & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'N';
            }
            else if (board->white_pieces.bishops & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'B';
            }
            else if (board->white_pieces.rooks & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'R';
            }
            else if (board->white_pieces.queens & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'Q';
            }
            else if (board->white_pieces.king & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'K';
            }
            else if (board->black_pieces.pawns & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'p';
            }
            else if (board->black_pieces.knights & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'n';
            }
            else if (board->black_pieces.bishops & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'b';
            }
            else if (board->black_pieces.rooks & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'r';
            }
            else if (board->black_pieces.queens & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'q';
            }
            else if (board->black_pieces.king & (1ULL << position))
            {
                if (empty_count > 0)
                {
                    fen[index++] = '0' + empty_count;
                    empty_count = 0;
                }
                fen[index++] = 'k';
            }
            else
            {
                empty_count++;
            }
        }
        if (empty_count > 0)
        {
            fen[index++] = '0' + empty_count;
        }
        if (rank > 0)
        {
            fen[index++] = '/';
        }
    }

    // Add side to move
    fen[index++] = ' ';
    fen[index++] = (board->side_to_move == WHITE) ? 'w' : 'b';

    // Add castling rights
    fen[index++] = ' ';
    if (board->castling_rights == 0)
    {
        fen[index++] = '-';
    }
    else
    {
        if (board->castling_rights & WHITE_KINGSIDE_CASTLE)
            fen[index++] = 'K';
        if (board->castling_rights & WHITE_QUEENSIDE_CASTLE)
            fen[index++] = 'Q';
        if (board->castling_rights & BLACK_KINGSIDE_CASTLE)
            fen[index++] = 'k';
        if (board->castling_rights & BLACK_QUEENSIDE_CASTLE)
            fen[index++] = 'q';
    }

    // Add en passant target square
    fen[index++] = ' ';
    if (board->en_passant)
    {
        int ep_square = ctzll(board->en_passant);
        fen[index++] = 'a' + (ep_square % 8);
        fen[index++] = '1' + (ep_square / 8);
    }
    else
    {
        fen[index++] = '-';
    }

    fen[index] = '\0'; // Null-terminate the string

    return fen;
}
