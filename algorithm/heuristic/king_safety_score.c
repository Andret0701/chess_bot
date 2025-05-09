#include "king_safety_score.h"
#include "../../utils/bitboard.h"
#include "heuristic_values.h"

double get_castling_score(BoardState *board_state, double game_phase)
{
    double score = 0;
    Board *board = &board_state->board;
    if (((board->castling_rights & WHITE_KINGSIDE_CASTLE) != 0) && ((board->castling_rights & WHITE_QUEENSIDE_CASTLE) != 0))
        score += CAN_CASTLE_BOTH_SIDES_MIDDLEGAME * (1 - game_phase) + CAN_CASTLE_BOTH_SIDES_ENDGAME * game_phase;
    else if ((board->castling_rights & WHITE_KINGSIDE_CASTLE) != 0)
        score += CAN_CASTLE_KINGSIDE_MIDDLEGAME * (1 - game_phase) + CAN_CASTLE_KINGSIDE_ENDGAME * game_phase;
    else if ((board->castling_rights & WHITE_QUEENSIDE_CASTLE) != 0)
        score += CAN_CASTLE_QUEENSIDE_MIDDLEGAME * (1 - game_phase) + CAN_CASTLE_QUEENSIDE_ENDGAME * game_phase;

    if (((board->castling_rights & BLACK_KINGSIDE_CASTLE) != 0) && ((board->castling_rights & BLACK_QUEENSIDE_CASTLE) != 0))
        score -= CAN_CASTLE_BOTH_SIDES_MIDDLEGAME * (1 - game_phase) + CAN_CASTLE_BOTH_SIDES_ENDGAME * game_phase;
    else if ((board->castling_rights & BLACK_KINGSIDE_CASTLE) != 0)
        score -= CAN_CASTLE_KINGSIDE_MIDDLEGAME * (1 - game_phase) + CAN_CASTLE_KINGSIDE_ENDGAME * game_phase;
    else if ((board->castling_rights & BLACK_QUEENSIDE_CASTLE) != 0)
        score -= CAN_CASTLE_QUEENSIDE_MIDDLEGAME * (1 - game_phase) + CAN_CASTLE_QUEENSIDE_ENDGAME * game_phase;

    if (board_state->has_castled & WHITE_KINGSIDE_CASTLE)
        score += HAS_CASTLED_KINGSIDE_MIDDLEGAME * (1 - game_phase) + HAS_CASTLED_KINGSIDE_ENDGAME * game_phase;

    if (board_state->has_castled & WHITE_QUEENSIDE_CASTLE)
        score += HAS_CASTLED_QUEENSIDE_MIDDLEGAME * (1 - game_phase) + HAS_CASTLED_QUEENSIDE_ENDGAME * game_phase;

    if (board_state->has_castled & BLACK_KINGSIDE_CASTLE)
        score -= HAS_CASTLED_KINGSIDE_MIDDLEGAME * (1 - game_phase) + HAS_CASTLED_KINGSIDE_ENDGAME * game_phase;

    if (board_state->has_castled & BLACK_QUEENSIDE_CASTLE)
        score -= HAS_CASTLED_QUEENSIDE_MIDDLEGAME * (1 - game_phase) + HAS_CASTLED_QUEENSIDE_ENDGAME * game_phase;

    return score;
}

double get_pawn_shelter_score(Board *board, double game_phase)
{
    double score = 0;

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

    // Score white pawn shelter
    if (has_white_front_pawn)
        score += FRONT_PAWN_BONUS_MIDDLEGAME * (1 - game_phase) + FRONT_PAWN_BONUS_ENDGAME * game_phase;

    if (has_white_ahead_pawn)
        score += AHEAD_PAWN_BONUS_MIDDLEGAME * (1 - game_phase) + AHEAD_PAWN_BONUS_ENDGAME * game_phase;
    if (has_white_left_pawn)
        score += LEFT_PAWN_BONUS_MIDDLEGAME * (1 - game_phase) + LEFT_PAWN_BONUS_ENDGAME * game_phase;
    if (has_white_right_pawn)
        score += RIGHT_PAWN_BONUS_MIDDLEGAME * (1 - game_phase) + RIGHT_PAWN_BONUS_ENDGAME * game_phase;

    // Score black pawn shelter (mirror of white scoring)
    if (has_black_front_pawn)
        score -= FRONT_PAWN_BONUS_MIDDLEGAME * (1 - game_phase) + FRONT_PAWN_BONUS_ENDGAME * game_phase;

    if (has_black_ahead_pawn)
        score -= AHEAD_PAWN_BONUS_MIDDLEGAME * (1 - game_phase) + AHEAD_PAWN_BONUS_ENDGAME * game_phase;
    if (has_black_left_pawn)
        score -= LEFT_PAWN_BONUS_MIDDLEGAME * (1 - game_phase) + LEFT_PAWN_BONUS_ENDGAME * game_phase;
    if (has_black_right_pawn)
        score -= RIGHT_PAWN_BONUS_MIDDLEGAME * (1 - game_phase) + RIGHT_PAWN_BONUS_ENDGAME * game_phase;

    return score;
}

double get_attacking_king_squares_score(BoardState *board_state, double game_phase)
{
    double score = 0;

    // Calculate squares adjacent to the kings, excluding the squares they currently occupy
    uint64_t white_king_squares = expand_bitboard(board_state->board.white_pieces.king) & ~board_state->board.white_pieces.king;
    uint64_t black_king_squares = expand_bitboard(board_state->board.black_pieces.king) & ~board_state->board.black_pieces.king;

    // Count the number of squares attacked by the opposing pieces
    uint8_t attacked_white_king_squares = __builtin_popcountll(white_king_squares & board_state->black_attack);
    uint8_t attacked_black_king_squares = __builtin_popcountll(black_king_squares & board_state->white_attack);

    score += ATTACKING_KING_SQUARES_MIDDLEGAME[attacked_white_king_squares] * (1 - game_phase) + ATTACKING_KING_SQUARES_ENDGAME[attacked_white_king_squares] * game_phase;
    score -= ATTACKING_KING_SQUARES_MIDDLEGAME[attacked_black_king_squares] * (1 - game_phase) + ATTACKING_KING_SQUARES_ENDGAME[attacked_black_king_squares] * game_phase;

    return score;
}

double get_king_safety_score(BoardState *board_state, double game_phase)
{
    double score = 0;
    score += get_castling_score(board_state, game_phase);
    score += get_pawn_shelter_score(&board_state->board, game_phase);
    score += get_attacking_king_squares_score(board_state, game_phase);
    return score;
}
