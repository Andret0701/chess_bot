#include "material_score.h"
// This is based on the Larry Kaufman's 2021 system
// https://en.wikipedia.org/wiki/Chess_piece_relative_value

typedef enum
{
    MIDDLEGAME,
    THRESHOLD,
    ENDGAME
} GamePhase;

GamePhase get_game_phase(Board *board)
{
    uint8_t white_queen_count = popcountll(board->white_pieces.queens);
    uint8_t black_queen_count = popcountll(board->black_pieces.queens);

    if (white_queen_count == 0 && black_queen_count == 0)
        return ENDGAME;

    if (white_queen_count == black_queen_count)
        return MIDDLEGAME;

    return THRESHOLD;
}

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
    uint64_t dark_squares = 0xAA55AA55AA55AA55ULL;
    uint64_t light_squares = 0x55AA55AA55AA55AAULL;

    bool has_dark_square_bishop = bishops & dark_squares;
    bool has_light_square_bishop = bishops & light_squares;

    return has_dark_square_bishop && has_light_square_bishop;
}

int32_t calculate_piece_score(uint64_t pieces, int32_t value)
{
    return __builtin_popcountll(pieces) * value;
}

int32_t calculate_combined_piece_score(uint64_t pieces, int32_t first_value, int32_t additional_value)
{
    int count = __builtin_popcountll(pieces);
    if (count == 0)
        return 0;

    return first_value + (count - 1) * additional_value;
}

int32_t get_middlegame_pawn_score(Board *board)
{
    uint64_t center_pawn_mask = 0x1818181818181818ULL;
    uint64_t bishop_pawn_mask = 0x2424242424242424ULL;
    uint64_t knight_pawn_mask = 0x0A0A0A0A0A0A0A0AULL;
    uint64_t rook_pawn_mask = 0x0101010101010101ULL;

    int32_t score = 0;
    score += calculate_piece_score(board->white_pieces.pawns & center_pawn_mask, 100);
    score -= calculate_piece_score(board->black_pieces.pawns & center_pawn_mask, 100);
    score += calculate_piece_score(board->white_pieces.pawns & bishop_pawn_mask, 95);
    score -= calculate_piece_score(board->black_pieces.pawns & bishop_pawn_mask, 95);
    score += calculate_piece_score(board->white_pieces.pawns & knight_pawn_mask, 85);
    score -= calculate_piece_score(board->black_pieces.pawns & knight_pawn_mask, 85);
    score += calculate_piece_score(board->white_pieces.pawns & rook_pawn_mask, 70);
    score -= calculate_piece_score(board->black_pieces.pawns & rook_pawn_mask, 70);

    return score;
}

int32_t get_middlegame_material_score(Board *board)
{
    int32_t score = 0;
    score += get_middlegame_pawn_score(board);

    score += calculate_piece_score(board->white_pieces.knights, 320);
    score -= calculate_piece_score(board->black_pieces.knights, 320);
    score += calculate_piece_score(board->white_pieces.bishops, 330);
    score -= calculate_piece_score(board->black_pieces.bishops, 330);
    score += calculate_combined_piece_score(board->white_pieces.rooks, 470, 450);
    score -= calculate_combined_piece_score(board->black_pieces.rooks, 470, 450);

    if (has_bishop_pair(board, WHITE))
        score += 30;
    if (has_bishop_pair(board, BLACK))
        score -= 30;

    return score;
}

int32_t get_threshold_material_score(Board *board)
{
    int32_t score = 0;
    score += calculate_piece_score(board->white_pieces.pawns, 90);
    score -= calculate_piece_score(board->black_pieces.pawns, 90);
    score += calculate_piece_score(board->white_pieces.knights, 320);
    score -= calculate_piece_score(board->black_pieces.knights, 320);
    score += calculate_piece_score(board->white_pieces.bishops, 330);
    score -= calculate_piece_score(board->black_pieces.bishops, 330);
    score += calculate_combined_piece_score(board->white_pieces.rooks, 480, 490);
    score -= calculate_combined_piece_score(board->black_pieces.rooks, 480, 490);
    score += calculate_combined_piece_score(board->white_pieces.queens, 940, 870);
    score -= calculate_combined_piece_score(board->black_pieces.queens, 940, 870);

    if (has_bishop_pair(board, WHITE))
        score += 40;
    if (has_bishop_pair(board, BLACK))
        score -= 40;

    return score;
}

int32_t get_endgame_material_score(Board *board)
{
    int32_t score = 0;
    score += calculate_piece_score(board->white_pieces.pawns, 100);
    score -= calculate_piece_score(board->black_pieces.pawns, 100);
    score += calculate_piece_score(board->white_pieces.knights, 320);
    score -= calculate_piece_score(board->black_pieces.knights, 320);
    score += calculate_piece_score(board->white_pieces.bishops, 330);
    score -= calculate_piece_score(board->black_pieces.bishops, 330);
    score += calculate_combined_piece_score(board->white_pieces.rooks, 530, 500);
    score -= calculate_combined_piece_score(board->black_pieces.rooks, 530, 500);

    if (has_bishop_pair(board, WHITE))
        score += 50;
    if (has_bishop_pair(board, BLACK))
        score -= 50;

    return score;
}

int32_t get_material_score(Board *board)
{
    GamePhase phase = get_game_phase(board);
    switch (phase)
    {
    case MIDDLEGAME:
        return get_middlegame_material_score(board);
    case THRESHOLD:
        return get_threshold_material_score(board);
    case ENDGAME:
        return get_endgame_material_score(board);
    }
    return 0;
}