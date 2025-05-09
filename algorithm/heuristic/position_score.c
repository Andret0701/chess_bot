#include "position_score.h"
#include "heuristic_values.h"

// Evaluate a single piece's position
static int evaluate_piece_position(uint64_t pieces, const double *middlegame_table, const double *endgame_table, bool is_white, double game_phase)
{
    if (is_white)
        pieces = flip_bitboard(pieces);
    int score = 0;
    while (pieces)
    {
        int square = __builtin_ctzll(pieces);
        score += middlegame_table[square] * (1 - game_phase) + endgame_table[square] * game_phase;
        pieces &= pieces - 1;
    }
    return score;
}

double get_position_score(Board *board, double game_phase)
{
    double score = 0;

    score += evaluate_piece_position(board->white_pieces.pawns, PAWN_TABLE_MIDDLEGAME, PAWN_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.pawns, PAWN_TABLE_MIDDLEGAME, PAWN_TABLE_ENDGAME, false, game_phase);
    score += evaluate_piece_position(board->white_pieces.king, KING_TABLE_MIDDLEGAME, KING_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.king, KING_TABLE_MIDDLEGAME, KING_TABLE_ENDGAME, false, game_phase);
    score += evaluate_piece_position(board->white_pieces.knights, KNIGHT_TABLE_MIDDLEGAME, KNIGHT_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.knights, KNIGHT_TABLE_MIDDLEGAME, KNIGHT_TABLE_ENDGAME, false, game_phase);
    score += evaluate_piece_position(board->white_pieces.bishops, BISHOP_TABLE_MIDDLEGAME, BISHOP_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.bishops, BISHOP_TABLE_MIDDLEGAME, BISHOP_TABLE_ENDGAME, false, game_phase);
    score += evaluate_piece_position(board->white_pieces.rooks, ROOK_TABLE_MIDDLEGAME, ROOK_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.rooks, ROOK_TABLE_MIDDLEGAME, ROOK_TABLE_ENDGAME, false, game_phase);
    score += evaluate_piece_position(board->white_pieces.queens, QUEEN_TABLE_MIDDLEGAME, QUEEN_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.queens, QUEEN_TABLE_MIDDLEGAME, QUEEN_TABLE_ENDGAME, false, game_phase);

    return score;
}