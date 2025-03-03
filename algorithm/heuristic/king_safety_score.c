#include "king_safety_score.h"
#include "../../utils/bitboard.h"

#define CAN_CASTLE_KINGSIDE_BONUS 15
#define CAN_CASTLE_QUEENSIDE_BONUS 10
#define CAN_CASTLE_BOTH_SIDES_BONUS 20

#define HAS_CASTLED_KINGSIDE_BONUS 26
#define HAS_CASTLED_QUEENSIDE_BONUS 21

int get_file(int square)
{
    return square % 8;
}

int get_rank(int square)
{
    return square / 8;
}

int get_king_square(uint64_t king)
{
    return __builtin_ctzll(king);
}

int32_t get_castling_score(BoardState *board_state)
{
    int32_t score = 0;
    Board *board = &board_state->board;
    if (((board->castling_rights & WHITE_KINGSIDE_CASTLE) != 0) && ((board->castling_rights & WHITE_QUEENSIDE_CASTLE) != 0))
        score += CAN_CASTLE_BOTH_SIDES_BONUS;
    else if ((board->castling_rights & WHITE_KINGSIDE_CASTLE) != 0)
        score += CAN_CASTLE_KINGSIDE_BONUS;
    else if ((board->castling_rights & WHITE_QUEENSIDE_CASTLE) != 0)
        score += CAN_CASTLE_QUEENSIDE_BONUS;

    if (((board->castling_rights & BLACK_KINGSIDE_CASTLE) != 0) && ((board->castling_rights & BLACK_QUEENSIDE_CASTLE) != 0))
        score -= CAN_CASTLE_BOTH_SIDES_BONUS;
    else if ((board->castling_rights & BLACK_KINGSIDE_CASTLE) != 0)
        score -= CAN_CASTLE_KINGSIDE_BONUS;
    else if ((board->castling_rights & BLACK_QUEENSIDE_CASTLE) != 0)
        score -= CAN_CASTLE_QUEENSIDE_BONUS;

    if (board_state->has_castled & WHITE_KINGSIDE_CASTLE)
        score += HAS_CASTLED_KINGSIDE_BONUS;

    if (board_state->has_castled & WHITE_QUEENSIDE_CASTLE)
        score += HAS_CASTLED_QUEENSIDE_BONUS;

    if (board_state->has_castled & BLACK_KINGSIDE_CASTLE)
        score -= HAS_CASTLED_KINGSIDE_BONUS;

    if (board_state->has_castled & BLACK_QUEENSIDE_CASTLE)
        score -= HAS_CASTLED_QUEENSIDE_BONUS;

    return score;
}

int32_t get_pawn_shelter_score(Board *board)
{
    int32_t score = 0;

    // White king shelter
    uint64_t front_of_white_king_mask = move_bitboard_up(board->white_pieces.king);
    uint64_t ahead_of_white_king_mask = move_bitboard_up(front_of_white_king_mask);
    uint64_t left_of_white_king_mask = move_bitboard_left(front_of_white_king_mask);
    uint64_t right_of_white_king_mask = move_bitboard_right(front_of_white_king_mask);

    // Black king shelter
    uint64_t front_of_black_king_mask = move_bitboard_down(board->black_pieces.king);
    uint64_t ahead_of_black_king_mask = move_bitboard_down(front_of_black_king_mask);
    uint64_t left_of_black_king_mask = move_bitboard_left(front_of_black_king_mask);
    uint64_t right_of_black_king_mask = move_bitboard_right(front_of_black_king_mask);

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
        score += 10;
    else
        score -= 15; // Penalty for missing front pawn

    if (has_white_ahead_pawn)
        score += 5;
    if (has_white_left_pawn)
        score += 8;
    if (has_white_right_pawn)
        score += 8;

    // Score black pawn shelter (mirror of white scoring)
    if (has_black_front_pawn)
        score -= 10;
    else
        score += 15;

    if (has_black_ahead_pawn)
        score -= 5;
    if (has_black_left_pawn)
        score -= 8;
    if (has_black_right_pawn)
        score -= 8;

    return score;
}

int32_t get_open_file_penalty(Board *board)
{
    int32_t score = 0;

    return score;
}

int32_t get_pawn_storm_score(Board *board) // Having pawns storming the enemy king is good
{
    int32_t score = 0;

    return score;
}

int32_t get_attacking_king_squares_score(BoardState *board_state)
{
    int32_t score = 0;

    // Calculate squares adjacent to the kings, excluding the squares they currently occupy
    uint64_t white_king_squares = expand_bitboard(board_state->board.white_pieces.king) & ~board_state->board.white_pieces.king;
    uint64_t black_king_squares = expand_bitboard(board_state->board.black_pieces.king) & ~board_state->board.black_pieces.king;

    // Count the number of squares attacked by the opposing pieces
    uint8_t attacked_white_king_squares = __builtin_popcountll(white_king_squares & board_state->black_attack);
    uint8_t attacked_black_king_squares = __builtin_popcountll(black_king_squares & board_state->white_attack);

    // Cap the number of attacked squares to 4
    if (attacked_white_king_squares > 4)
        attacked_white_king_squares = 4;

    if (attacked_black_king_squares > 4)
        attacked_black_king_squares = 4;

    // Apply exponential scoring for attacking squares around the enemy king
    if (attacked_black_king_squares > 0)
        score += 1 << (attacked_black_king_squares - 1);

    if (attacked_white_king_squares > 0)
        score -= 1 << (attacked_white_king_squares - 1);

    return score;
}

int32_t get_weak_back_rank_penalty(Board *board) // Having a weak back rank is bad
{
    int32_t score = 0;

    return score;
}

int32_t get_king_safety_score(BoardState *board_state)
{
    int32_t score = 0;
    score += get_castling_score(board_state);
    score += get_pawn_shelter_score(&board_state->board);
    score += get_open_file_penalty(&board_state->board);
    score += get_pawn_storm_score(&board_state->board);
    score += get_attacking_king_squares_score(board_state);
    score += get_weak_back_rank_penalty(&board_state->board);
    return score;
}