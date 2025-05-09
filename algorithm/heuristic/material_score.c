#include "material_score.h"
#include "../../utils/bitboard.h"
#include "heuristic_values.h"

bool has_bishop_pair(Board *board, Color color)
{
    uint64_t bishops;
    if (color == WHITE)
        bishops = board->white_pieces.bishops;
    else
        bishops = board->black_pieces.bishops;

    if (__builtin_popcountll(bishops) < 2)
        return false;

    // Check if bishops are on different colors
    bool has_dark_square_bishop = bishops & DARK_SQUARES_MASK;
    bool has_light_square_bishop = bishops & LIGHT_SQUARES_MASK;

    return has_dark_square_bishop && has_light_square_bishop;
}

double get_material_score(Board *board, double game_phase)
{
    double score = 0;

    score += __builtin_popcountll(board->white_pieces.pawns & CENTER_FILES_MASK) * ((1 - game_phase) * CENTER_PAWN_MIDDLEGAME + game_phase * CENTER_PAWN_ENDGAME);
    score -= __builtin_popcountll(board->black_pieces.pawns & CENTER_FILES_MASK) * ((1 - game_phase) * CENTER_PAWN_MIDDLEGAME + game_phase * CENTER_PAWN_ENDGAME);
    score += __builtin_popcountll(board->white_pieces.pawns & BISHOP_FILES_MASK) * ((1 - game_phase) * BISHOP_PAWN_MIDDLEGAME + game_phase * BISHOP_PAWN_ENDGAME);
    score -= __builtin_popcountll(board->black_pieces.pawns & BISHOP_FILES_MASK) * ((1 - game_phase) * BISHOP_PAWN_MIDDLEGAME + game_phase * BISHOP_PAWN_ENDGAME);
    score += __builtin_popcountll(board->white_pieces.pawns & KNIGHT_FILES_MASK) * ((1 - game_phase) * KNIGHT_PAWN_MIDDLEGAME + game_phase * KNIGHT_PAWN_ENDGAME);
    score -= __builtin_popcountll(board->black_pieces.pawns & KNIGHT_FILES_MASK) * ((1 - game_phase) * KNIGHT_PAWN_MIDDLEGAME + game_phase * KNIGHT_PAWN_ENDGAME);
    score += __builtin_popcountll(board->white_pieces.pawns & ROOK_FILES_MASK) * ((1 - game_phase) * ROOK_PAWN_MIDDLEGAME + game_phase * ROOK_PAWN_ENDGAME);
    score -= __builtin_popcountll(board->black_pieces.pawns & ROOK_FILES_MASK) * ((1 - game_phase) * ROOK_PAWN_MIDDLEGAME + game_phase * ROOK_PAWN_ENDGAME);
    score += __builtin_popcountll(board->white_pieces.knights) * ((1 - game_phase) * KNIGHT_MIDDLEGAME + game_phase * KNIGHT_ENDGAME);
    score -= __builtin_popcountll(board->black_pieces.knights) * ((1 - game_phase) * KNIGHT_MIDDLEGAME + game_phase * KNIGHT_ENDGAME);
    score += __builtin_popcountll(board->white_pieces.bishops) * ((1 - game_phase) * BISHOP_MIDDLEGAME + game_phase * BISHOP_ENDGAME);
    score -= __builtin_popcountll(board->black_pieces.bishops) * ((1 - game_phase) * BISHOP_MIDDLEGAME + game_phase * BISHOP_ENDGAME);
    score += (__builtin_popcountll(board->white_pieces.rooks) > 0) ? FIRST_ROOK_MIDDLEGAME * (1 - game_phase) + FIRST_ROOK_ENDGAME * game_phase : 0;
    score -= (__builtin_popcountll(board->black_pieces.rooks) > 0) ? FIRST_ROOK_MIDDLEGAME * (1 - game_phase) + FIRST_ROOK_ENDGAME * game_phase : 0;
    score += (__builtin_popcountll(board->white_pieces.rooks) > 1) ? (__builtin_popcountll(board->white_pieces.rooks) - 1) * ((1 - game_phase) * ADDITIONAL_ROOK_MIDDLEGAME + game_phase * ADDITIONAL_ROOK_ENDGAME) : 0;
    score -= (__builtin_popcountll(board->black_pieces.rooks) > 1) ? (__builtin_popcountll(board->black_pieces.rooks) - 1) * ((1 - game_phase) * ADDITIONAL_ROOK_MIDDLEGAME + game_phase * ADDITIONAL_ROOK_ENDGAME) : 0;
    score += (__builtin_popcountll(board->white_pieces.queens) > 0) ? FIRST_QUEEN_MIDDLEGAME * (1 - game_phase) + FIRST_QUEEN_ENDGAME * game_phase : 0;
    score -= (__builtin_popcountll(board->black_pieces.queens) > 0) ? FIRST_QUEEN_MIDDLEGAME * (1 - game_phase) + FIRST_QUEEN_ENDGAME * game_phase : 0;
    score += (__builtin_popcountll(board->white_pieces.queens) > 1) ? (__builtin_popcountll(board->white_pieces.queens) - 1) * ((1 - game_phase) * ADDITIONAL_QUEEN_MIDDLEGAME + game_phase * ADDITIONAL_QUEEN_ENDGAME) : 0;
    score -= (__builtin_popcountll(board->black_pieces.queens) > 1) ? (__builtin_popcountll(board->black_pieces.queens) - 1) * ((1 - game_phase) * ADDITIONAL_QUEEN_MIDDLEGAME + game_phase * ADDITIONAL_QUEEN_ENDGAME) : 0;
    score += has_bishop_pair(board, WHITE) ? BISHOP_PAIR_MIDDLEGAME * (1 - game_phase) + BISHOP_PAIR_ENDGAME * game_phase : 0;
    score -= has_bishop_pair(board, BLACK) ? BISHOP_PAIR_MIDDLEGAME * (1 - game_phase) + BISHOP_PAIR_ENDGAME * game_phase : 0;

    return score;
}