#include "position_score.h"
#include "piece_square_tables.h"

// Evaluate a single piece's position
static int evaluate_piece_position(uint64_t pieces, const int16_t *table, bool is_white)
{
    if (is_white)
        pieces = flip_bitboard(pieces);
    int score = 0;
    while (pieces)
    {
        int square = __builtin_ctzll(pieces);
        score += table[square];
        pieces &= pieces - 1;
    }
    return score;
}

bool is_endgame(Board *board)
{
    return popcountll(board->white_pieces.queens) + popcountll(board->black_pieces.queens) <= 1;
}

int32_t get_position_score(Board *board)
{
    int32_t score = 0;
    bool endgame = is_endgame(board);

    // Pawn positioning (different for middlegame/endgame)
    const int16_t *pawn_table = endgame ? PAWN_TABLE_END : PAWN_TABLE;
    score += evaluate_piece_position(board->white_pieces.pawns, pawn_table, true);
    score -= evaluate_piece_position(board->black_pieces.pawns, pawn_table, false);

    // King positioning (different for middlegame/endgame)
    const int16_t *king_table = endgame ? KING_TABLE_END : KING_TABLE;
    score += evaluate_piece_position(board->white_pieces.king, king_table, true);
    score -= evaluate_piece_position(board->black_pieces.king, king_table, false);

    // Positional scoring
    score += evaluate_piece_position(board->white_pieces.knights, KNIGHT_TABLE, true);
    score -= evaluate_piece_position(board->black_pieces.knights, KNIGHT_TABLE, false);
    score += evaluate_piece_position(board->white_pieces.bishops, BISHOP_TABLE, true);
    score -= evaluate_piece_position(board->black_pieces.bishops, BISHOP_TABLE, false);
    score += evaluate_piece_position(board->white_pieces.rooks, ROOK_TABLE, true);
    score -= evaluate_piece_position(board->black_pieces.rooks, ROOK_TABLE, false);
    score += evaluate_piece_position(board->white_pieces.queens, QUEEN_TABLE, true);
    score -= evaluate_piece_position(board->black_pieces.queens, QUEEN_TABLE, false);

    return score;
}