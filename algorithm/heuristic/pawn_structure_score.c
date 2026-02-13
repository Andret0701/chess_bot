#include "pawn_structure_score.h"
#include "../../utils/bitboard.h"
#include "heuristic_values.h"
#include "../../engine/attack_generation/attack_generation.h"

int32_t get_double_pawn_score(BoardState *board_state, uint8_t middlegame_phase, uint8_t endgame_phase)
{
    int32_t score = 0;

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

    uint8_t white_double_pawns = 0;
    uint8_t black_double_pawns = 0;

    white_double_pawns += (white_pawn_a_file > 1) ? (white_pawn_a_file - 1) : 0;
    white_double_pawns += (white_pawn_b_file > 1) ? (white_pawn_b_file - 1) : 0;
    white_double_pawns += (white_pawn_c_file > 1) ? (white_pawn_c_file - 1) : 0;
    white_double_pawns += (white_pawn_d_file > 1) ? (white_pawn_d_file - 1) : 0;
    white_double_pawns += (white_pawn_e_file > 1) ? (white_pawn_e_file - 1) : 0;
    white_double_pawns += (white_pawn_f_file > 1) ? (white_pawn_f_file - 1) : 0;
    white_double_pawns += (white_pawn_g_file > 1) ? (white_pawn_g_file - 1) : 0;
    white_double_pawns += (white_pawn_h_file > 1) ? (white_pawn_h_file - 1) : 0;

    black_double_pawns += (black_pawn_a_file > 1) ? (black_pawn_a_file - 1) : 0;
    black_double_pawns += (black_pawn_b_file > 1) ? (black_pawn_b_file - 1) : 0;
    black_double_pawns += (black_pawn_c_file > 1) ? (black_pawn_c_file - 1) : 0;
    black_double_pawns += (black_pawn_d_file > 1) ? (black_pawn_d_file - 1) : 0;
    black_double_pawns += (black_pawn_e_file > 1) ? (black_pawn_e_file - 1) : 0;
    black_double_pawns += (black_pawn_f_file > 1) ? (black_pawn_f_file - 1) : 0;
    black_double_pawns += (black_pawn_g_file > 1) ? (black_pawn_g_file - 1) : 0;
    black_double_pawns += (black_pawn_h_file > 1) ? (black_pawn_h_file - 1) : 0;

    score += white_double_pawns * (DOUBLE_PAWN_MIDDLEGAME * middlegame_phase + DOUBLE_PAWN_ENDGAME * endgame_phase);
    score -= black_double_pawns * (DOUBLE_PAWN_MIDDLEGAME * middlegame_phase + DOUBLE_PAWN_ENDGAME * endgame_phase);

    return score;
}

int32_t get_isolated_pawn_score(BoardState *board_state, uint8_t middlegame_phase, uint8_t endgame_phase)
{
    int32_t score = 0;

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

    uint8_t white_isolated_pawns = 0;
    uint8_t black_isolated_pawns = 0;

    white_isolated_pawns += file_a_has_isolated_white_pawn ? 1 : 0;
    white_isolated_pawns += file_b_has_isolated_white_pawn ? 1 : 0;
    white_isolated_pawns += file_c_has_isolated_white_pawn ? 1 : 0;
    white_isolated_pawns += file_d_has_isolated_white_pawn ? 1 : 0;
    white_isolated_pawns += file_e_has_isolated_white_pawn ? 1 : 0;
    white_isolated_pawns += file_f_has_isolated_white_pawn ? 1 : 0;
    white_isolated_pawns += file_g_has_isolated_white_pawn ? 1 : 0;
    white_isolated_pawns += file_h_has_isolated_white_pawn ? 1 : 0;

    black_isolated_pawns += file_a_has_isolated_black_pawn ? 1 : 0;
    black_isolated_pawns += file_b_has_isolated_black_pawn ? 1 : 0;
    black_isolated_pawns += file_c_has_isolated_black_pawn ? 1 : 0;
    black_isolated_pawns += file_d_has_isolated_black_pawn ? 1 : 0;
    black_isolated_pawns += file_e_has_isolated_black_pawn ? 1 : 0;
    black_isolated_pawns += file_f_has_isolated_black_pawn ? 1 : 0;
    black_isolated_pawns += file_g_has_isolated_black_pawn ? 1 : 0;
    black_isolated_pawns += file_h_has_isolated_black_pawn ? 1 : 0;

    score += white_isolated_pawns * (ISOLATED_PAWN_MIDDLEGAME * middlegame_phase + ISOLATED_PAWN_ENDGAME * endgame_phase);
    score -= black_isolated_pawns * (ISOLATED_PAWN_MIDDLEGAME * middlegame_phase + ISOLATED_PAWN_ENDGAME * endgame_phase);

    return score;
}

int32_t get_backward_pawn_score(BoardState *board_state, uint8_t middlegame_phase, uint8_t endgame_phase)
{
    int32_t score = 0;

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

    score += number_of_white_backward_pawns * (BACKWARD_PAWN_MIDDLEGAME * middlegame_phase + BACKWARD_PAWN_ENDGAME * endgame_phase);
    score -= number_of_black_backward_pawns * (BACKWARD_PAWN_MIDDLEGAME * middlegame_phase + BACKWARD_PAWN_ENDGAME * endgame_phase);

    return score;
}

int32_t get_passed_pawn_score(BoardState *board_state)
{
    int32_t score = 0;

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
            score += PASSED_PAWN_BONUS[number_of_squares_from_promotion];
    }

    while (black_pawns)
    {
        int index = __builtin_ctzll(black_pawns);
        black_pawns &= black_pawns - 1;

        uint64_t position = 1ULL << index;
        uint64_t passed_pawn_mask = get_passed_pawn_mask_black(position);
        uint8_t number_of_squares_from_promotion = index / 8;
        if (!(passed_pawn_mask & board_state->board.white_pieces.pawns))
            score -= PASSED_PAWN_BONUS[number_of_squares_from_promotion];
    }

    return score * 24;
}

int32_t get_pawn_chain_score(BoardState *board_state, uint8_t middlegame_phase, uint8_t endgame_phase)
{
    int32_t score = 0;

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

    score += __builtin_popcountll(protected_white_pawns) * (PROTECTED_PAWN_MIDDLEGAME * middlegame_phase + PROTECTED_PAWN_ENDGAME * endgame_phase);
    score -= __builtin_popcountll(protected_black_pawns) * (PROTECTED_PAWN_MIDDLEGAME * middlegame_phase + PROTECTED_PAWN_ENDGAME * endgame_phase);
    score += __builtin_popcountll(protected_by_protected_white_pawns) * (PROTECTED_BY_PROTECTED_PAWN_MIDDLEGAME * middlegame_phase + PROTECTED_BY_PROTECTED_PAWN_ENDGAME * endgame_phase);
    score -= __builtin_popcountll(protected_by_protected_black_pawns) * (PROTECTED_BY_PROTECTED_PAWN_MIDDLEGAME * middlegame_phase + PROTECTED_BY_PROTECTED_PAWN_ENDGAME * endgame_phase);

    return score;
}

int32_t get_pawn_structure_score(BoardState *board_state, uint8_t middlegame_phase, uint8_t endgame_phase)
{
    int32_t score = 0;

    score += get_double_pawn_score(board_state, middlegame_phase, endgame_phase);
    score += get_isolated_pawn_score(board_state, middlegame_phase, endgame_phase);
    score += get_backward_pawn_score(board_state, middlegame_phase, endgame_phase);
    score += get_passed_pawn_score(board_state);
    score += get_pawn_chain_score(board_state, middlegame_phase, endgame_phase);

    return score;
}