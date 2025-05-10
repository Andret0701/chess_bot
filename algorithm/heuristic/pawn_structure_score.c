#include "pawn_structure_score.h"
#include "../../utils/bitboard.h"
#include "heuristic_values.h"

double get_double_pawn_penalty(BoardState *board_state, double game_phase)
{
    double score = 0;

    uint64_t white_pawns = board_state->board.white_pieces.pawns;
    uint64_t black_pawns = board_state->board.black_pieces.pawns;

    uint8_t white_pawn_a_file = __builtin_popcountll(white_pawns & FILE_A_MASK);
    uint8_t white_pawn_b_file = __builtin_popcountll(white_pawns & FILE_B_MASK);
    uint8_t white_pawn_c_file = __builtin_popcountll(white_pawns & FILE_C_MASK);
    uint8_t white_pawn_d_file = __builtin_popcountll(white_pawns & FILE_D_MASK);
    uint8_t white_pawn_e_file = __builtin_popcountll(white_pawns & FILE_E_MASK);
    uint8_t white_pawn_f_file = __builtin_popcountll(white_pawns & FILE_F_MASK);
    uint8_t white_pawn_g_file = __builtin_popcountll(white_pawns & FILE_G_MASK);
    uint8_t white_pawn_h_file = __builtin_popcountll(white_pawns & FILE_H_MASK);

    uint8_t black_pawn_a_file = __builtin_popcountll(black_pawns & FILE_A_MASK);
    uint8_t black_pawn_b_file = __builtin_popcountll(black_pawns & FILE_B_MASK);
    uint8_t black_pawn_c_file = __builtin_popcountll(black_pawns & FILE_C_MASK);
    uint8_t black_pawn_d_file = __builtin_popcountll(black_pawns & FILE_D_MASK);
    uint8_t black_pawn_e_file = __builtin_popcountll(black_pawns & FILE_E_MASK);
    uint8_t black_pawn_f_file = __builtin_popcountll(black_pawns & FILE_F_MASK);
    uint8_t black_pawn_g_file = __builtin_popcountll(black_pawns & FILE_G_MASK);
    uint8_t black_pawn_h_file = __builtin_popcountll(black_pawns & FILE_H_MASK);

    double double_pawn_penalty = (1 - game_phase) * DOUBLE_PAWN_MIDDLEGAME + game_phase * DOUBLE_PAWN_ENDGAME;
    score += (white_pawn_a_file > 1) ? (white_pawn_a_file - 1) * double_pawn_penalty : 0;
    score += (white_pawn_b_file > 1) ? (white_pawn_b_file - 1) * double_pawn_penalty : 0;
    score += (white_pawn_c_file > 1) ? (white_pawn_c_file - 1) * double_pawn_penalty : 0;
    score += (white_pawn_d_file > 1) ? (white_pawn_d_file - 1) * double_pawn_penalty : 0;
    score += (white_pawn_e_file > 1) ? (white_pawn_e_file - 1) * double_pawn_penalty : 0;
    score += (white_pawn_f_file > 1) ? (white_pawn_f_file - 1) * double_pawn_penalty : 0;
    score += (white_pawn_g_file > 1) ? (white_pawn_g_file - 1) * double_pawn_penalty : 0;
    score += (white_pawn_h_file > 1) ? (white_pawn_h_file - 1) * double_pawn_penalty : 0;

    score -= (black_pawn_a_file > 1) ? (black_pawn_a_file - 1) * double_pawn_penalty : 0;
    score -= (black_pawn_b_file > 1) ? (black_pawn_b_file - 1) * double_pawn_penalty : 0;
    score -= (black_pawn_c_file > 1) ? (black_pawn_c_file - 1) * double_pawn_penalty : 0;
    score -= (black_pawn_d_file > 1) ? (black_pawn_d_file - 1) * double_pawn_penalty : 0;
    score -= (black_pawn_e_file > 1) ? (black_pawn_e_file - 1) * double_pawn_penalty : 0;
    score -= (black_pawn_f_file > 1) ? (black_pawn_f_file - 1) * double_pawn_penalty : 0;
    score -= (black_pawn_g_file > 1) ? (black_pawn_g_file - 1) * double_pawn_penalty : 0;
    score -= (black_pawn_h_file > 1) ? (black_pawn_h_file - 1) * double_pawn_penalty : 0;

    return score;
}

double get_isolated_pawn_penalty(BoardState *board_state, double game_phase)
{
    double score = 0;

    uint64_t white_pawns = board_state->board.white_pieces.pawns;
    uint64_t black_pawns = board_state->board.black_pieces.pawns;

    bool file_a_has_isolated_white_pawn = __builtin_popcountll(white_pawns & FILE_A_MASK) == 1 && __builtin_popcountll(white_pawns & FILE_B_MASK) == 0;
    bool file_b_has_isolated_white_pawn = __builtin_popcountll(white_pawns & FILE_B_MASK) == 1 && __builtin_popcountll(white_pawns & FILE_A_MASK) == 0 && __builtin_popcountll(white_pawns & FILE_C_MASK) == 0;
    bool file_c_has_isolated_white_pawn = __builtin_popcountll(white_pawns & FILE_C_MASK) == 1 && __builtin_popcountll(white_pawns & FILE_B_MASK) == 0 && __builtin_popcountll(white_pawns & FILE_D_MASK) == 0;
    bool file_d_has_isolated_white_pawn = __builtin_popcountll(white_pawns & FILE_D_MASK) == 1 && __builtin_popcountll(white_pawns & FILE_C_MASK) == 0 && __builtin_popcountll(white_pawns & FILE_E_MASK) == 0;
    bool file_e_has_isolated_white_pawn = __builtin_popcountll(white_pawns & FILE_E_MASK) == 1 && __builtin_popcountll(white_pawns & FILE_D_MASK) == 0 && __builtin_popcountll(white_pawns & FILE_F_MASK) == 0;
    bool file_f_has_isolated_white_pawn = __builtin_popcountll(white_pawns & FILE_F_MASK) == 1 && __builtin_popcountll(white_pawns & FILE_E_MASK) == 0 && __builtin_popcountll(white_pawns & FILE_G_MASK) == 0;
    bool file_g_has_isolated_white_pawn = __builtin_popcountll(white_pawns & FILE_G_MASK) == 1 && __builtin_popcountll(white_pawns & FILE_F_MASK) == 0 && __builtin_popcountll(white_pawns & FILE_H_MASK) == 0;
    bool file_h_has_isolated_white_pawn = __builtin_popcountll(white_pawns & FILE_H_MASK) == 1 && __builtin_popcountll(white_pawns & FILE_G_MASK) == 0;

    bool file_a_has_isolated_black_pawn = __builtin_popcountll(black_pawns & FILE_A_MASK) == 1 && __builtin_popcountll(black_pawns & FILE_B_MASK) == 0;
    bool file_b_has_isolated_black_pawn = __builtin_popcountll(black_pawns & FILE_B_MASK) == 1 && __builtin_popcountll(black_pawns & FILE_A_MASK) == 0 && __builtin_popcountll(black_pawns & FILE_C_MASK) == 0;
    bool file_c_has_isolated_black_pawn = __builtin_popcountll(black_pawns & FILE_C_MASK) == 1 && __builtin_popcountll(black_pawns & FILE_B_MASK) == 0 && __builtin_popcountll(black_pawns & FILE_D_MASK) == 0;
    bool file_d_has_isolated_black_pawn = __builtin_popcountll(black_pawns & FILE_D_MASK) == 1 && __builtin_popcountll(black_pawns & FILE_C_MASK) == 0 && __builtin_popcountll(black_pawns & FILE_E_MASK) == 0;
    bool file_e_has_isolated_black_pawn = __builtin_popcountll(black_pawns & FILE_E_MASK) == 1 && __builtin_popcountll(black_pawns & FILE_D_MASK) == 0 && __builtin_popcountll(black_pawns & FILE_F_MASK) == 0;
    bool file_f_has_isolated_black_pawn = __builtin_popcountll(black_pawns & FILE_F_MASK) == 1 && __builtin_popcountll(black_pawns & FILE_E_MASK) == 0 && __builtin_popcountll(black_pawns & FILE_G_MASK) == 0;
    bool file_g_has_isolated_black_pawn = __builtin_popcountll(black_pawns & FILE_G_MASK) == 1 && __builtin_popcountll(black_pawns & FILE_F_MASK) == 0 && __builtin_popcountll(black_pawns & FILE_H_MASK) == 0;
    bool file_h_has_isolated_black_pawn = __builtin_popcountll(black_pawns & FILE_H_MASK) == 1 && __builtin_popcountll(black_pawns & FILE_G_MASK) == 0;

    double isolated_pawn_penalty = (1 - game_phase) * ISOLATED_PAWN_MIDDLEGAME + game_phase * ISOLATED_PAWN_ENDGAME;
    score += file_a_has_isolated_white_pawn ? isolated_pawn_penalty : 0;
    score += file_b_has_isolated_white_pawn ? isolated_pawn_penalty : 0;
    score += file_c_has_isolated_white_pawn ? isolated_pawn_penalty : 0;
    score += file_d_has_isolated_white_pawn ? isolated_pawn_penalty : 0;
    score += file_e_has_isolated_white_pawn ? isolated_pawn_penalty : 0;
    score += file_f_has_isolated_white_pawn ? isolated_pawn_penalty : 0;
    score += file_g_has_isolated_white_pawn ? isolated_pawn_penalty : 0;
    score += file_h_has_isolated_white_pawn ? isolated_pawn_penalty : 0;

    score -= file_a_has_isolated_black_pawn ? isolated_pawn_penalty : 0;
    score -= file_b_has_isolated_black_pawn ? isolated_pawn_penalty : 0;
    score -= file_c_has_isolated_black_pawn ? isolated_pawn_penalty : 0;
    score -= file_d_has_isolated_black_pawn ? isolated_pawn_penalty : 0;
    score -= file_e_has_isolated_black_pawn ? isolated_pawn_penalty : 0;
    score -= file_f_has_isolated_black_pawn ? isolated_pawn_penalty : 0;
    score -= file_g_has_isolated_black_pawn ? isolated_pawn_penalty : 0;
    score -= file_h_has_isolated_black_pawn ? isolated_pawn_penalty : 0;

    return score;
}

double get_backward_pawn_penalty(BoardState *board_state, double game_phase)
{
    double score = 0;

    uint64_t white_pawns = board_state->board.white_pieces.pawns;
    uint64_t black_pawns = board_state->board.black_pieces.pawns;

    uint8_t number_of_white_backward_pawns = 0;
    while (white_pawns)
    {
        int index = __builtin_ctzll(white_pawns);
        white_pawns &= white_pawns - 1;

        uint64_t position = 1ULL << index;
        uint64_t backward_pawn_mask = get_backward_pawn_mask_white(position);

        bool is_pawn_undefendable = (backward_pawn_mask & board_state->board.white_pieces.pawns) == 0;

        uint64_t front_square = increment_rank(position);
        bool is_front_under_attack = (front_square & board_state->black_attacks.pawns) != 0;

        bool has_black_pawn_in_front = (get_white_front_file_mask(position) & board_state->board.black_pieces.pawns) != 0;

        if (is_pawn_undefendable && is_front_under_attack && !has_black_pawn_in_front)
            number_of_white_backward_pawns++;
    }

    uint8_t number_of_black_backward_pawns = 0;
    while (black_pawns)
    {
        int index = __builtin_ctzll(black_pawns);
        black_pawns &= black_pawns - 1;

        uint64_t position = 1ULL << index;
        uint64_t backward_pawn_mask = get_backward_pawn_mask_black(position);
        bool is_pawn_undefendable = (backward_pawn_mask & board_state->board.black_pieces.pawns) == 0;

        uint64_t front_square = decrement_rank(position);
        bool is_front_under_attack = (front_square & board_state->white_attacks.pawns) != 0;

        bool has_white_pawn_in_front = (get_black_front_file_mask(position) & board_state->board.white_pieces.pawns) != 0;

        if (is_pawn_undefendable && is_front_under_attack && !has_white_pawn_in_front)
            number_of_black_backward_pawns++;
    }

    double backward_pawn_penalty = (1 - game_phase) * BACKWARD_PAWN_MIDDLEGAME + game_phase * BACKWARD_PAWN_ENDGAME;
    score += number_of_white_backward_pawns * backward_pawn_penalty;
    score -= number_of_black_backward_pawns * backward_pawn_penalty;

    return score;
}

double get_passed_pawn_bonus(BoardState *board_state, double game_phase)
{
    double score = 0;

    uint64_t white_pawns = board_state->board.white_pieces.pawns;
    uint64_t black_pawns = board_state->board.black_pieces.pawns;

    while (white_pawns)
    {
        int index = __builtin_ctzll(white_pawns);
        white_pawns &= white_pawns - 1;

        uint64_t position = 1ULL << index;
        uint64_t passed_pawn_mask = get_passed_pawn_mask_white(position);
        uint8_t number_of_squares_from_promotion = 7 - (index / 8);
        if (!(passed_pawn_mask & board_state->board.black_pieces.pawns))
            score += PASSED_PAWN_MIDDLEGAME[number_of_squares_from_promotion] * (1 - game_phase) + PASSED_PAWN_ENDGAME[number_of_squares_from_promotion] * game_phase;
    }

    while (black_pawns)
    {
        int index = __builtin_ctzll(black_pawns);
        black_pawns &= black_pawns - 1;

        uint64_t position = 1ULL << index;
        uint64_t passed_pawn_mask = get_passed_pawn_mask_black(position);
        uint8_t number_of_squares_from_promotion = index / 8;
        if (!(passed_pawn_mask & board_state->board.white_pieces.pawns))
            score -= PASSED_PAWN_MIDDLEGAME[number_of_squares_from_promotion] * (1 - game_phase) + PASSED_PAWN_ENDGAME[number_of_squares_from_promotion] * game_phase;
    }

    return score;
}

double get_pawn_chain_bonus(BoardState *board_state, double game_phase)
{
    double score = 0;
    uint64_t white_pawn_attacks = board_state->white_attacks.pawns;
    uint64_t black_pawn_attacks = board_state->black_attacks.pawns;

    uint64_t protected_white_pawns = board_state->board.white_pieces.pawns & white_pawn_attacks;
    uint64_t protected_black_pawns = board_state->board.black_pieces.pawns & black_pawn_attacks;

    uint64_t protected_white_pawn_attacks = generate_white_pawn_attacks(protected_white_pawns);
    uint64_t protected_black_pawn_attacks = generate_black_pawn_attacks(protected_black_pawns);

    uint64_t protected_by_protected_white_pawns = protected_white_pawn_attacks & board_state->board.white_pieces.pawns;
    uint64_t protected_by_protected_black_pawns = protected_black_pawn_attacks & board_state->board.black_pieces.pawns;

    protected_white_pawns &= ~protected_by_protected_white_pawns;
    protected_black_pawns &= ~protected_by_protected_black_pawns;

    double protected_pawn_bonus = (1 - game_phase) * PROTECTED_PAWN_MIDDLEGAME + game_phase * PROTECTED_PAWN_ENDGAME;
    double protected_by_protected_pawn_bonus = (1 - game_phase) * PROTECTED_BY_PROTECTED_PAWN_MIDDLEGAME + game_phase * PROTECTED_BY_PROTECTED_PAWN_ENDGAME;

    score += __builtin_popcountll(protected_white_pawns) * protected_pawn_bonus;
    score -= __builtin_popcountll(protected_black_pawns) * protected_pawn_bonus;
    score += __builtin_popcountll(protected_by_protected_white_pawns) * protected_by_protected_pawn_bonus;
    score -= __builtin_popcountll(protected_by_protected_black_pawns) * protected_by_protected_pawn_bonus;

    return score;
}

double get_bishop_block_score(BoardState *board_state, double game_phase)
{
    double score = 0;

    bool white_has_light_square_bishop = (board_state->board.white_pieces.bishops & LIGHT_SQUARES_MASK) != 0;
    bool white_has_dark_square_bishop = (board_state->board.white_pieces.bishops & DARK_SQUARES_MASK) != 0;
    bool black_has_light_square_bishop = (board_state->board.black_pieces.bishops & LIGHT_SQUARES_MASK) != 0;
    bool black_has_dark_square_bishop = (board_state->board.black_pieces.bishops & DARK_SQUARES_MASK) != 0;

    uint8_t white_pawn_light_square_count = __builtin_popcountll(board_state->board.white_pieces.pawns & LIGHT_SQUARES_MASK);
    uint8_t white_pawn_dark_square_count = __builtin_popcountll(board_state->board.white_pieces.pawns & DARK_SQUARES_MASK);
    uint8_t black_pawn_light_square_count = __builtin_popcountll(board_state->board.black_pieces.pawns & LIGHT_SQUARES_MASK);
    uint8_t black_pawn_dark_square_count = __builtin_popcountll(board_state->board.black_pieces.pawns & DARK_SQUARES_MASK);

    double bishop_pawn_friendly_penalty = (1 - game_phase) * BISHOP_PAWN_FRIENDLY_MIDDLEGAME + game_phase * BISHOP_PAWN_FRIENDLY_ENDGAME;
    double bishop_pawn_enemy_penalty = (1 - game_phase) * BISHOP_PAWN_ENEMY_MIDDLEGAME + game_phase * BISHOP_PAWN_ENEMY_ENDGAME;
    if (white_has_light_square_bishop)
    {
        score -= white_pawn_light_square_count * bishop_pawn_friendly_penalty;
        score -= black_pawn_light_square_count * bishop_pawn_enemy_penalty;
    }

    if (white_has_dark_square_bishop)
    {
        score -= white_pawn_dark_square_count * bishop_pawn_friendly_penalty;
        score -= black_pawn_dark_square_count * bishop_pawn_enemy_penalty;
    }

    if (black_has_light_square_bishop)
    {
        score += white_pawn_light_square_count * bishop_pawn_enemy_penalty;
        score += black_pawn_light_square_count * bishop_pawn_friendly_penalty;
    }

    if (black_has_dark_square_bishop)
    {
        score += white_pawn_dark_square_count * bishop_pawn_enemy_penalty;
        score += black_pawn_dark_square_count * bishop_pawn_friendly_penalty;
    }

    return score;
}

double get_pawn_structure_score(BoardState *board_state, double game_phase)
{
    double score = 0;
    score += get_double_pawn_penalty(board_state, game_phase);
    score += get_isolated_pawn_penalty(board_state, game_phase);
    score += get_backward_pawn_penalty(board_state, game_phase);
    score += get_passed_pawn_bonus(board_state, game_phase);
    score += get_pawn_chain_bonus(board_state, game_phase);
    score += get_bishop_block_score(board_state, game_phase);
    return score;
}