#include "piece_moves.h"
#include "attack_generation/attack_generation.h"

static inline void init_board(BoardState *board_state)
{
    board_state->white_pieces = board_state->board.white_pieces.pawns | board_state->board.white_pieces.knights | board_state->board.white_pieces.bishops | board_state->board.white_pieces.rooks | board_state->board.white_pieces.queens | board_state->board.white_pieces.king;
    board_state->black_pieces = board_state->board.black_pieces.pawns | board_state->board.black_pieces.knights | board_state->board.black_pieces.bishops | board_state->board.black_pieces.rooks | board_state->board.black_pieces.queens | board_state->board.black_pieces.king;
    board_state->occupied = board_state->white_pieces | board_state->black_pieces;

    // check for castling rights
    if (board_state->board.castling_rights & WHITE_KINGSIDE_CASTLE && (board_state->board.white_pieces.rooks & position_to_bitboard(7, 0)) == 0)
        board_state->board.castling_rights &= ~WHITE_KINGSIDE_CASTLE;
    if (board_state->board.castling_rights & WHITE_QUEENSIDE_CASTLE && (board_state->board.white_pieces.rooks & position_to_bitboard(0, 0)) == 0)
        board_state->board.castling_rights &= ~WHITE_QUEENSIDE_CASTLE;
    if (board_state->board.castling_rights & BLACK_KINGSIDE_CASTLE && (board_state->board.black_pieces.rooks & position_to_bitboard(7, 7)) == 0)
        board_state->board.castling_rights &= ~BLACK_KINGSIDE_CASTLE;
    if (board_state->board.castling_rights & BLACK_QUEENSIDE_CASTLE && (board_state->board.black_pieces.rooks & position_to_bitboard(0, 7)) == 0)
        board_state->board.castling_rights &= ~BLACK_QUEENSIDE_CASTLE;

    generate_attacks(board_state);

    board_state->white_check = board_state->black_attack & board_state->board.white_pieces.king;
    board_state->black_check = board_state->white_attack & board_state->board.black_pieces.king;
}

static inline void validate_white_move(BoardStack *stack)
{
    if (__builtin_expect(!stack->boards[stack->count].white_check, 1))
        stack->count++;
}

static inline void validate_black_move(BoardStack *stack)
{
    if (__builtin_expect(!stack->boards[stack->count].black_check, 1))
        stack->count++;
}

static inline void remove_white_piece(BoardState *board_state, uint8_t x, uint8_t y)
{
    uint64_t position = position_to_bitboard(x, y);
    board_state->board.white_pieces.pawns &= ~position;
    board_state->board.white_pieces.knights &= ~position;
    board_state->board.white_pieces.bishops &= ~position;
    board_state->board.white_pieces.rooks &= ~position;
    board_state->board.white_pieces.queens &= ~position;
}
static inline void remove_black_piece(BoardState *board_state, uint8_t x, uint8_t y)
{
    uint64_t position = position_to_bitboard(x, y);
    board_state->board.black_pieces.pawns &= ~position;
    board_state->board.black_pieces.knights &= ~position;
    board_state->board.black_pieces.bishops &= ~position;
    board_state->board.black_pieces.rooks &= ~position;
    board_state->board.black_pieces.queens &= ~position;
}

static inline bool is_white_piece(BoardState *board_state, uint8_t x, uint8_t y)
{
    uint64_t position = position_to_bitboard(x, y);
    return (board_state->white_pieces & position) != 0;
}

static inline bool is_black_piece(BoardState *board_state, uint8_t x, uint8_t y)
{
    uint64_t position = position_to_bitboard(x, y);
    return (board_state->black_pieces & position) != 0;
}

void generate_moves(BoardState *board_state, BoardStack *stack)
{
    if (board_state->board.side_to_move == WHITE)
    {
        // Pawns
        uint64_t pawns = board_state->board.white_pieces.pawns;
        while (pawns)
        {
            uint8_t sq = __builtin_ctzll(pawns);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_white_pawn_moves(board_state, x, y, stack);
            pawns &= pawns - 1;
        }
        // Knights
        uint64_t knights = board_state->board.white_pieces.knights;
        while (knights)
        {
            uint8_t sq = __builtin_ctzll(knights);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_white_knight_moves(board_state, x, y, stack);
            knights &= knights - 1;
        }
        // Bishops
        uint64_t bishops = board_state->board.white_pieces.bishops;
        while (bishops)
        {
            uint8_t sq = __builtin_ctzll(bishops);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_white_bishop_moves(board_state, x, y, stack);
            bishops &= bishops - 1;
        }
        // Rooks
        uint64_t rooks = board_state->board.white_pieces.rooks;
        while (rooks)
        {
            uint8_t sq = __builtin_ctzll(rooks);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_white_rook_moves(board_state, x, y, stack);
            rooks &= rooks - 1;
        }
        // Queens
        uint64_t queens = board_state->board.white_pieces.queens;
        while (queens)
        {
            uint8_t sq = __builtin_ctzll(queens);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_white_queen_moves(board_state, x, y, stack);
            queens &= queens - 1;
        }
        // King
        uint8_t sq = __builtin_ctzll(board_state->board.white_pieces.king);
        generate_white_king_moves(board_state, sq % 8, sq / 8, stack);
    }
    else if (board_state->board.side_to_move == BLACK)
    {
        // Pawns
        uint64_t pawns = board_state->board.black_pieces.pawns;
        while (pawns)
        {
            uint8_t sq = __builtin_ctzll(pawns);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_black_pawn_moves(board_state, x, y, stack);
            pawns &= pawns - 1;
        }
        // Knights
        uint64_t knights = board_state->board.black_pieces.knights;
        while (knights)
        {
            uint8_t sq = __builtin_ctzll(knights);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_black_knight_moves(board_state, x, y, stack);
            knights &= knights - 1;
        }
        // Bishops
        uint64_t bishops = board_state->board.black_pieces.bishops;
        while (bishops)
        {
            uint8_t sq = __builtin_ctzll(bishops);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_black_bishop_moves(board_state, x, y, stack);
            bishops &= bishops - 1;
        }
        // Rooks
        uint64_t rooks = board_state->board.black_pieces.rooks;
        while (rooks)
        {
            uint8_t sq = __builtin_ctzll(rooks);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_black_rook_moves(board_state, x, y, stack);
            rooks &= rooks - 1;
        }
        // Queens
        uint64_t queens = board_state->board.black_pieces.queens;
        while (queens)
        {
            uint8_t sq = __builtin_ctzll(queens);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_black_queen_moves(board_state, x, y, stack);
            queens &= queens - 1;
        }
        // King
        uint8_t sq = __builtin_ctzll(board_state->board.black_pieces.king);
        generate_black_king_moves(board_state, sq % 8, sq / 8, stack);
    }
}

void generate_captures(BoardState *board_state, BoardStack *stack)
{
    if (board_state->board.side_to_move == WHITE)
    {
        // Pawns
        uint64_t pawns = board_state->board.white_pieces.pawns;
        while (pawns)
        {
            uint8_t sq = __builtin_ctzll(pawns);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_white_pawn_captures(board_state, x, y, stack);
            pawns &= pawns - 1;
        }
        // Knights
        uint64_t knights = board_state->board.white_pieces.knights;
        while (knights)
        {
            uint8_t sq = __builtin_ctzll(knights);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_white_knight_captures(board_state, x, y, stack);
            knights &= knights - 1;
        }
        // Bishops
        uint64_t bishops = board_state->board.white_pieces.bishops;
        while (bishops)
        {
            uint8_t sq = __builtin_ctzll(bishops);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_white_bishop_captures(board_state, x, y, stack);
            bishops &= bishops - 1;
        }
        // Rooks
        uint64_t rooks = board_state->board.white_pieces.rooks;
        while (rooks)
        {
            uint8_t sq = __builtin_ctzll(rooks);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_white_rook_captures(board_state, x, y, stack);
            rooks &= rooks - 1;
        }
        // Queens
        uint64_t queens = board_state->board.white_pieces.queens;
        while (queens)
        {
            uint8_t sq = __builtin_ctzll(queens);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_white_queen_captures(board_state, x, y, stack);
            queens &= queens - 1;
        }
        // King
        uint64_t king = board_state->board.white_pieces.king;
        uint8_t sq = __builtin_ctzll(king);
        uint8_t x = sq % 8;
        uint8_t y = sq / 8;
        generate_white_king_captures(board_state, x, y, stack);
    }
    else if (board_state->board.side_to_move == BLACK)
    {
        // Pawns
        uint64_t pawns = board_state->board.black_pieces.pawns;
        while (pawns)
        {
            uint8_t sq = __builtin_ctzll(pawns);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_black_pawn_captures(board_state, x, y, stack);
            pawns &= pawns - 1;
        }
        // Knights
        uint64_t knights = board_state->board.black_pieces.knights;
        while (knights)
        {
            uint8_t sq = __builtin_ctzll(knights);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_black_knight_captures(board_state, x, y, stack);
            knights &= knights - 1;
        }
        // Bishops
        uint64_t bishops = board_state->board.black_pieces.bishops;
        while (bishops)
        {
            uint8_t sq = __builtin_ctzll(bishops);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_black_bishop_captures(board_state, x, y, stack);
            bishops &= bishops - 1;
        }
        // Rooks
        uint64_t rooks = board_state->board.black_pieces.rooks;
        while (rooks)
        {
            uint8_t sq = __builtin_ctzll(rooks);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_black_rook_captures(board_state, x, y, stack);
            rooks &= rooks - 1;
        }
        // Queens
        uint64_t queens = board_state->board.black_pieces.queens;
        while (queens)
        {
            uint8_t sq = __builtin_ctzll(queens);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            generate_black_queen_captures(board_state, x, y, stack);
            queens &= queens - 1;
        }
        // King
        uint64_t king = board_state->board.black_pieces.king;
        uint8_t sq = __builtin_ctzll(king);
        uint8_t x = sq % 8;
        uint8_t y = sq / 8;
        generate_black_king_captures(board_state, x, y, stack);
    }
}

bool is_finished(BoardState *board_state)
{
    if (board_state->board.side_to_move == WHITE)
    {
        // Pawns
        uint64_t pawns = board_state->board.white_pieces.pawns;
        while (pawns)
        {
            uint8_t sq = __builtin_ctzll(pawns);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (white_pawn_can_move(board_state, x, y))
                return false;
            pawns &= pawns - 1;
        }
        // Knights
        uint64_t knights = board_state->board.white_pieces.knights;
        while (knights)
        {
            uint8_t sq = __builtin_ctzll(knights);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (white_knight_can_move(board_state, x, y))
                return false;
            knights &= knights - 1;
        }
        // Bishops
        uint64_t bishops = board_state->board.white_pieces.bishops;
        while (bishops)
        {
            uint8_t sq = __builtin_ctzll(bishops);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (white_bishop_can_move(board_state, x, y))
                return false;
            bishops &= bishops - 1;
        }
        // Rooks
        uint64_t rooks = board_state->board.white_pieces.rooks;
        while (rooks)
        {
            uint8_t sq = __builtin_ctzll(rooks);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (white_rook_can_move(board_state, x, y))
                return false;
            rooks &= rooks - 1;
        }
        // Queens
        uint64_t queens = board_state->board.white_pieces.queens;
        while (queens)
        {
            uint8_t sq = __builtin_ctzll(queens);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (white_queen_can_move(board_state, x, y))
                return false;
            queens &= queens - 1;
        }
        // King
        uint64_t king = board_state->board.white_pieces.king;
        if (king)
        {
            uint8_t sq = __builtin_ctzll(king);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (white_king_can_move(board_state, x, y))
                return false;
        }
    }
    else if (board_state->board.side_to_move == BLACK)
    {
        // Pawns
        uint64_t pawns = board_state->board.black_pieces.pawns;
        while (pawns)
        {
            uint8_t sq = __builtin_ctzll(pawns);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (black_pawn_can_move(board_state, x, y))
                return false;
            pawns &= pawns - 1;
        }
        // Knights
        uint64_t knights = board_state->board.black_pieces.knights;
        while (knights)
        {
            uint8_t sq = __builtin_ctzll(knights);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (black_knight_can_move(board_state, x, y))
                return false;
            knights &= knights - 1;
        }
        // Bishops
        uint64_t bishops = board_state->board.black_pieces.bishops;
        while (bishops)
        {
            uint8_t sq = __builtin_ctzll(bishops);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (black_bishop_can_move(board_state, x, y))
                return false;
            bishops &= bishops - 1;
        }
        // Rooks
        uint64_t rooks = board_state->board.black_pieces.rooks;
        while (rooks)
        {
            uint8_t sq = __builtin_ctzll(rooks);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (black_rook_can_move(board_state, x, y))
                return false;
            rooks &= rooks - 1;
        }
        // Queens
        uint64_t queens = board_state->board.black_pieces.queens;
        while (queens)
        {
            uint8_t sq = __builtin_ctzll(queens);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (black_queen_can_move(board_state, x, y))
                return false;
            queens &= queens - 1;
        }
        // King
        uint64_t king = board_state->board.black_pieces.king;
        if (king)
        {
            uint8_t sq = __builtin_ctzll(king);
            uint8_t x = sq % 8;
            uint8_t y = sq / 8;
            if (black_king_can_move(board_state, x, y))
                return false;
        }
    }
    return true;
}