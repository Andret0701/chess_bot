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

MaterialFeatures get_material_features(Board *board, double game_phase)
{
    MaterialFeatures features;

    features.center_pawns = create_feature(
        __builtin_popcountll(board->white_pieces.pawns & CENTER_FILES_MASK),
        __builtin_popcountll(board->black_pieces.pawns & CENTER_FILES_MASK),
        game_phase);
    features.bishop_pawns = create_feature(
        __builtin_popcountll(board->white_pieces.pawns & BISHOP_FILES_MASK),
        __builtin_popcountll(board->black_pieces.pawns & BISHOP_FILES_MASK),
        game_phase);
    features.knight_pawns = create_feature(
        __builtin_popcountll(board->white_pieces.pawns & KNIGHT_FILES_MASK),
        __builtin_popcountll(board->black_pieces.pawns & KNIGHT_FILES_MASK),
        game_phase);
    features.rook_pawns = create_feature(
        __builtin_popcountll(board->white_pieces.pawns & ROOK_FILES_MASK),
        __builtin_popcountll(board->black_pieces.pawns & ROOK_FILES_MASK),
        game_phase);
    features.knights = create_feature(
        __builtin_popcountll(board->white_pieces.knights),
        __builtin_popcountll(board->black_pieces.knights),
        game_phase);
    features.bishops = create_feature(
        __builtin_popcountll(board->white_pieces.bishops),
        __builtin_popcountll(board->black_pieces.bishops),
        game_phase);
    features.first_rook = create_feature(
        (__builtin_popcountll(board->white_pieces.rooks) > 0) ? 1.0 : 0.0,
        (__builtin_popcountll(board->black_pieces.rooks) > 0) ? 1.0 : 0.0,
        game_phase);
    features.additional_rooks = create_feature(
        (__builtin_popcountll(board->white_pieces.rooks) > 1) ? (__builtin_popcountll(board->white_pieces.rooks) - 1) : 0.0,
        (__builtin_popcountll(board->black_pieces.rooks) > 1) ? (__builtin_popcountll(board->black_pieces.rooks) - 1) : 0.0,
        game_phase);
    features.first_queen = create_feature(
        (__builtin_popcountll(board->white_pieces.queens) > 0) ? 1.0 : 0.0,
        (__builtin_popcountll(board->black_pieces.queens) > 0) ? 1.0 : 0.0,
        game_phase);
    features.additional_queens = create_feature(
        (__builtin_popcountll(board->white_pieces.queens) > 1) ? (__builtin_popcountll(board->white_pieces.queens) - 1) : 0.0,
        (__builtin_popcountll(board->black_pieces.queens) > 1) ? (__builtin_popcountll(board->black_pieces.queens) - 1) : 0.0,
        game_phase);
    features.bishop_pair = create_feature(
        has_bishop_pair(board, WHITE) ? 1.0 : 0.0,
        has_bishop_pair(board, BLACK) ? 1.0 : 0.0,
        game_phase);

    return features;
}