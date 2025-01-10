#include "king_safety_score.h"

#define FILE_A 0x0101010101010101
#define FILE_H 0x8080808080808080
#define RANK_1 0x00000000000000FF
#define RANK_8 0xFF00000000000000

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

int32_t get_castling_rights_score(Board *board) // This should be changed. Having castled is better than having the right to castle
{
    int32_t score = 0;
    if (board->castling_rights & WHITE_KINGSIDE_CASTLE)
        score += 10;
    if (board->castling_rights & WHITE_QUEENSIDE_CASTLE)
        score += 10;
    if (board->castling_rights & BLACK_KINGSIDE_CASTLE)
        score -= 10;
    if (board->castling_rights & BLACK_QUEENSIDE_CASTLE)
        score -= 10;

    return score;
}

int32_t get_pawn_shelter_score(Board *board)
{
    int32_t score = 0;

    // White king shelter
    uint64_t front_of_white_king_mask = (board->white_pieces.king & ~RANK_8) << 8;
    uint64_t ahead_of_white_king_mask = (front_of_white_king_mask & ~RANK_8) << 8;
    uint64_t left_of_white_king_mask = (front_of_white_king_mask & ~FILE_A) << 1;
    uint64_t right_of_white_king_mask = (front_of_white_king_mask & ~FILE_H) >> 1;

    // Black king shelter
    uint64_t front_of_black_king_mask = (board->black_pieces.king & ~RANK_1) >> 8;
    uint64_t ahead_of_black_king_mask = (front_of_black_king_mask & ~RANK_1) >> 8;
    uint64_t left_of_black_king_mask = (front_of_black_king_mask & ~FILE_A) << 1;
    uint64_t right_of_black_king_mask = (front_of_black_king_mask & ~FILE_H) >> 1;

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

    // Bonus for connected pawns
    if (has_white_left_pawn && has_white_front_pawn)
        score += 3;
    if (has_white_right_pawn && has_white_front_pawn)
        score += 3;

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

    // Bonus for connected pawns
    if (has_black_left_pawn && has_black_front_pawn)
        score -= 3;
    if (has_black_right_pawn && has_black_front_pawn)
        score -= 3;

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

int32_t get_attacking_king_squares_score(Board *board) // Having pieces attacking squares around the enemy king is good
{
    int32_t score = 0;

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
    score += get_castling_rights_score(&board_state->board);
    score += get_pawn_shelter_score(&board_state->board);
    score += get_open_file_penalty(&board_state->board);
    score += get_pawn_storm_score(&board_state->board);
    score += get_attacking_king_squares_score(&board_state->board);
    score += get_weak_back_rank_penalty(&board_state->board);
    return score;
}