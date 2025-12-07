#include "king_safety_score.h"
#include "../../utils/bitboard.h"
#include "heuristic_values.h"

CastlingFeatures get_castling_features(BoardState *board_state, double game_phase)
{
    Board *board = &board_state->board;

    Feature can_castle_both_sides = create_feature(
        (((board->castling_rights & WHITE_KINGSIDE_CASTLE) != 0) && ((board->castling_rights & WHITE_QUEENSIDE_CASTLE) != 0)) ? 1.0 : 0.0,
        (((board->castling_rights & BLACK_KINGSIDE_CASTLE) != 0) && ((board->castling_rights & BLACK_QUEENSIDE_CASTLE) != 0)) ? 1.0 : 0.0,
        game_phase);

    Feature can_castle_kingside = create_feature(
        ((board->castling_rights & WHITE_KINGSIDE_CASTLE) != 0) ? 1.0 : 0.0,
        ((board->castling_rights & BLACK_KINGSIDE_CASTLE) != 0) ? 1.0 : 0.0,
        game_phase);

    Feature can_castle_queenside = create_feature(
        ((board->castling_rights & WHITE_QUEENSIDE_CASTLE) != 0) ? 1.0 : 0.0,
        ((board->castling_rights & BLACK_QUEENSIDE_CASTLE) != 0) ? 1.0 : 0.0,
        game_phase);

    Feature has_castled_kingside = create_feature(
        (board_state->has_castled & WHITE_KINGSIDE_CASTLE) ? 1.0 : 0.0,
        (board_state->has_castled & BLACK_KINGSIDE_CASTLE) ? 1.0 : 0.0,
        game_phase);

    Feature has_castled_queenside = create_feature(
        (board_state->has_castled & WHITE_QUEENSIDE_CASTLE) ? 1.0 : 0.0,
        (board_state->has_castled & BLACK_QUEENSIDE_CASTLE) ? 1.0 : 0.0,
        game_phase);

    return (CastlingFeatures){
        can_castle_both_sides,
        can_castle_kingside,
        can_castle_queenside,
        has_castled_kingside,
        has_castled_queenside};
}

PawnShelterFeatures get_pawn_shelter_features(Board *board, double game_phase)
{
    // White king shelter
    uint64_t front_of_white_king_mask = increment_rank(board->white_pieces.king);
    uint64_t ahead_of_white_king_mask = increment_rank(front_of_white_king_mask);
    uint64_t left_of_white_king_mask = decrement_file(front_of_white_king_mask);
    uint64_t right_of_white_king_mask = increment_file(front_of_white_king_mask);

    // Black king shelter
    uint64_t front_of_black_king_mask = decrement_rank(board->black_pieces.king);
    uint64_t ahead_of_black_king_mask = decrement_rank(front_of_black_king_mask);
    uint64_t left_of_black_king_mask = decrement_file(front_of_black_king_mask);
    uint64_t right_of_black_king_mask = increment_file(front_of_black_king_mask);

    // Check white pawn shelter
    bool has_white_front_pawn = (board->white_pieces.pawns & front_of_white_king_mask) != 0;
    bool has_white_ahead_pawn = (board->white_pieces.pawns & ahead_of_white_king_mask) != 0;
    bool has_white_left_pawn = (board->white_pieces.pawns & left_of_white_king_mask) != 0;
    bool has_white_right_pawn = (board->white_pieces.pawns & right_of_white_king_mask) != 0;

    // Check black pawn shelter
    bool has_black_front_pawn = (board->black_pieces.pawns & front_of_black_king_mask) != 0;
    bool has_black_ahead_pawn = (board->black_pieces.pawns & ahead_of_black_king_mask) != 0;
    bool has_black_left_pawn = (board->black_pieces.pawns & left_of_black_king_mask) != 0;
    bool has_black_right_pawn = (board->black_pieces.pawns & right_of_black_king_mask) != 0;

    return (PawnShelterFeatures){
        create_feature(
            has_white_front_pawn ? 1.0 : 0.0,
            has_black_front_pawn ? 1.0 : 0.0,
            game_phase),
        create_feature(
            has_white_ahead_pawn ? 1.0 : 0.0,
            has_black_ahead_pawn ? 1.0 : 0.0,
            game_phase),
        create_feature(
            has_white_left_pawn ? 1.0 : 0.0,
            has_black_left_pawn ? 1.0 : 0.0,
            game_phase),
        create_feature(
            has_white_right_pawn ? 1.0 : 0.0,
            has_black_right_pawn ? 1.0 : 0.0,
            game_phase)};
}

Feature get_attacking_king_squares_feature(BoardState *board_state, double game_phase)
{
    // Calculate squares adjacent to the kings, excluding the squares they currently occupy
    uint64_t white_king_squares = expand_bitboard(board_state->board.white_pieces.king) & ~board_state->board.white_pieces.king;
    uint64_t black_king_squares = expand_bitboard(board_state->board.black_pieces.king) & ~board_state->board.black_pieces.king;

    // Count the number of squares attacked by the opposing pieces
    uint8_t attacked_white_king_squares = __builtin_popcountll(white_king_squares & board_state->black_attack);
    uint8_t attacked_black_king_squares = __builtin_popcountll(black_king_squares & board_state->white_attack);

    attacked_white_king_squares *= attacked_white_king_squares;
    attacked_black_king_squares *= attacked_black_king_squares;

    return create_feature(attacked_white_king_squares, attacked_black_king_squares, game_phase);
}

KingSafetyFeatures get_king_safety_features(BoardState *board_state, double game_phase)
{
    CastlingFeatures castling = get_castling_features(board_state, game_phase);
    PawnShelterFeatures pawn_shelter = get_pawn_shelter_features(&board_state->board, game_phase);
    Feature attacking_king_squares = get_attacking_king_squares_feature(board_state, game_phase);
    return (KingSafetyFeatures){
        castling,
        pawn_shelter,
        attacking_king_squares};
}
