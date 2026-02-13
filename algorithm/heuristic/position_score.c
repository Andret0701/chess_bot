#include "position_score.h"
#include "heuristic_values.h"
#include <stdlib.h>
#include "../../utils/bitboard.h"

// Evaluate a single piece's position
int32_t evaluate_piece_position(uint64_t pieces, const int32_t *middlegame_table, const int32_t *endgame_table, bool is_white, uint8_t middlegame_phase, uint8_t endgame_phase)
{
    if (is_white)
        pieces = flip_bitboard(pieces);
    int32_t score = 0;
    while (pieces)
    {
        int square = __builtin_ctzll(pieces);
        score += middlegame_table[square] * middlegame_phase + endgame_table[square] * endgame_phase;
        pieces &= pieces - 1;
    }
    return score;
}

int32_t get_position_score(Board *board, uint8_t middlegame_phase, uint8_t endgame_phase)
{
    int32_t score = 0;

    score += evaluate_piece_position(board->white_pieces.pawns, PAWN_TABLE_MIDDLEGAME, PAWN_TABLE_ENDGAME, true, middlegame_phase, endgame_phase);
    score -= evaluate_piece_position(board->black_pieces.pawns, PAWN_TABLE_MIDDLEGAME, PAWN_TABLE_ENDGAME, false, middlegame_phase, endgame_phase);
    score += evaluate_piece_position(board->white_pieces.king, KING_TABLE_MIDDLEGAME, KING_TABLE_ENDGAME, true, middlegame_phase, endgame_phase);
    score -= evaluate_piece_position(board->black_pieces.king, KING_TABLE_MIDDLEGAME, KING_TABLE_ENDGAME, false, middlegame_phase, endgame_phase);
    score += evaluate_piece_position(board->white_pieces.knights, KNIGHT_TABLE_MIDDLEGAME, KNIGHT_TABLE_ENDGAME, true, middlegame_phase, endgame_phase);
    score -= evaluate_piece_position(board->black_pieces.knights, KNIGHT_TABLE_MIDDLEGAME, KNIGHT_TABLE_ENDGAME, false, middlegame_phase, endgame_phase);
    score += evaluate_piece_position(board->white_pieces.bishops, BISHOP_TABLE_MIDDLEGAME, BISHOP_TABLE_ENDGAME, true, middlegame_phase, endgame_phase);
    score -= evaluate_piece_position(board->black_pieces.bishops, BISHOP_TABLE_MIDDLEGAME, BISHOP_TABLE_ENDGAME, false, middlegame_phase, endgame_phase);
    score += evaluate_piece_position(board->white_pieces.rooks, ROOK_TABLE_MIDDLEGAME, ROOK_TABLE_ENDGAME, true, middlegame_phase, endgame_phase);
    score -= evaluate_piece_position(board->black_pieces.rooks, ROOK_TABLE_MIDDLEGAME, ROOK_TABLE_ENDGAME, false, middlegame_phase, endgame_phase);
    score += evaluate_piece_position(board->white_pieces.queens, QUEEN_TABLE_MIDDLEGAME, QUEEN_TABLE_ENDGAME, true, middlegame_phase, endgame_phase);
    score -= evaluate_piece_position(board->black_pieces.queens, QUEEN_TABLE_MIDDLEGAME, QUEEN_TABLE_ENDGAME, false, middlegame_phase, endgame_phase);

    return score;
}