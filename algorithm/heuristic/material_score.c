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

int32_t get_material_score(Board *board, uint8_t middlegame_phase, uint8_t endgame_phase)
{
    int32_t score = 0;

    score += (__builtin_popcountll(board->white_pieces.pawns & CENTER_FILES_MASK) - __builtin_popcountll(board->black_pieces.pawns & CENTER_FILES_MASK)) * (CENTER_PAWN_MIDDLEGAME * middlegame_phase + CENTER_PAWN_ENDGAME * endgame_phase);
    score += (__builtin_popcountll(board->white_pieces.pawns & BISHOP_FILES_MASK) - __builtin_popcountll(board->black_pieces.pawns & BISHOP_FILES_MASK)) * (BISHOP_PAWN_MIDDLEGAME * middlegame_phase + BISHOP_PAWN_ENDGAME * endgame_phase);
    score += (__builtin_popcountll(board->white_pieces.pawns & KNIGHT_FILES_MASK) - __builtin_popcountll(board->black_pieces.pawns & KNIGHT_FILES_MASK)) * (KNIGHT_PAWN_MIDDLEGAME * middlegame_phase + KNIGHT_PAWN_ENDGAME * endgame_phase);
    score += (__builtin_popcountll(board->white_pieces.pawns & ROOK_FILES_MASK) - __builtin_popcountll(board->black_pieces.pawns & ROOK_FILES_MASK)) * (ROOK_PAWN_MIDDLEGAME * middlegame_phase + ROOK_PAWN_ENDGAME * endgame_phase);

    score += (__builtin_popcountll(board->white_pieces.knights) - __builtin_popcountll(board->black_pieces.knights)) * (KNIGHT_MIDDLEGAME * middlegame_phase + KNIGHT_ENDGAME * endgame_phase);
    score += (__builtin_popcountll(board->white_pieces.bishops) - __builtin_popcountll(board->black_pieces.bishops)) * (BISHOP_MIDDLEGAME * middlegame_phase + BISHOP_ENDGAME * endgame_phase);
    score += ((__builtin_popcountll(board->white_pieces.rooks) > 0 ? 1 : 0) - (__builtin_popcountll(board->black_pieces.rooks) > 0 ? 1 : 0)) * (FIRST_ROOK_MIDDLEGAME * middlegame_phase + FIRST_ROOK_ENDGAME * endgame_phase);
    score += ((__builtin_popcountll(board->white_pieces.rooks) > 1 ? __builtin_popcountll(board->white_pieces.rooks) - 1 : 0) - (__builtin_popcountll(board->black_pieces.rooks) > 1 ? __builtin_popcountll(board->black_pieces.rooks) - 1 : 0)) * (ADDITIONAL_ROOK_MIDDLEGAME * middlegame_phase + ADDITIONAL_ROOK_ENDGAME * endgame_phase);
    score += ((__builtin_popcountll(board->white_pieces.queens) > 0 ? 1 : 0) - (__builtin_popcountll(board->black_pieces.queens) > 0 ? 1 : 0)) * (FIRST_QUEEN_MIDDLEGAME * middlegame_phase + FIRST_QUEEN_ENDGAME * endgame_phase);
    score += ((__builtin_popcountll(board->white_pieces.queens) > 1 ? __builtin_popcountll(board->white_pieces.queens) - 1 : 0) - (__builtin_popcountll(board->black_pieces.queens) > 1 ? __builtin_popcountll(board->black_pieces.queens) - 1 : 0)) * (ADDITIONAL_QUEEN_MIDDLEGAME * middlegame_phase + ADDITIONAL_QUEEN_ENDGAME * endgame_phase);

    if (has_bishop_pair(board, WHITE))
        score += BISHOP_PAIR_MIDDLEGAME * middlegame_phase + BISHOP_PAIR_ENDGAME * endgame_phase;
    if (has_bishop_pair(board, BLACK))
        score -= BISHOP_PAIR_MIDDLEGAME * middlegame_phase + BISHOP_PAIR_ENDGAME * endgame_phase;

    return score;
}