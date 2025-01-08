#include "position_score.h"
#include "piece_square_tables.h"

// Flip table for getting white's perspective
static const int flip_table[64] = {
    56, 57, 58, 59, 60, 61, 62, 63,
    48, 49, 50, 51, 52, 53, 54, 55,
    40, 41, 42, 43, 44, 45, 46, 47,
    32, 33, 34, 35, 36, 37, 38, 39,
    24, 25, 26, 27, 28, 29, 30, 31,
    16, 17, 18, 19, 20, 21, 22, 23,
    8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7};

// Evaluate a single piece's position
static int evaluate_piece_position(uint64_t pieces, const int16_t *table, bool is_white)
{
    int score = 0;
    while (pieces)
    {
        int square = __builtin_ctzll(pieces);
        score += table[is_white ? flip_table[square] : square];
        pieces &= pieces - 1;
    }
    return score;
}

int32_t get_position_score(Board *board, GamePhase game_phase)
{
    int32_t score = 0;

    // Positional scoring
    const int16_t *pawn_table = (game_phase == ENDGAME) ? pawn_table_end : pawn_table;
    score += evaluate_piece_position(board->white_pieces.pawns, pawn_table, true);
    score -= evaluate_piece_position(board->black_pieces.pawns, pawn_table, false);
    score += evaluate_piece_position(board->white_pieces.knights, knight_table, true);
    score -= evaluate_piece_position(board->black_pieces.knights, knight_table, false);
    score += evaluate_piece_position(board->white_pieces.bishops, bishop_table, true);
    score -= evaluate_piece_position(board->black_pieces.bishops, bishop_table, false);
    score += evaluate_piece_position(board->white_pieces.rooks, rook_table, true);
    score -= evaluate_piece_position(board->black_pieces.rooks, rook_table, false);
    score += evaluate_piece_position(board->white_pieces.queens, queen_table, true);
    score -= evaluate_piece_position(board->black_pieces.queens, queen_table, false);

    // King positioning (different for middlegame/endgame)
    const int16_t *king_table = (game_phase == ENDGAME) ? king_table_end : king_table;
    score += evaluate_piece_position(board->white_pieces.king, king_table, true);
    score -= evaluate_piece_position(board->black_pieces.king, king_table, false);

    return score;
}