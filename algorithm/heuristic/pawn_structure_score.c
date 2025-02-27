#include "pawn_structure_score.h"
#include "../../utils/bitboard.h"

#define DOUBLE_PAWN_PENALTY 5
#define PAWN_CHAIN_BONUS 10

int32_t get_double_pawn_penalty(BoardState *board_state)
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

    score -= (white_pawn_a_file > 1) ? (white_pawn_a_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score -= (white_pawn_b_file > 1) ? (white_pawn_b_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score -= (white_pawn_c_file > 1) ? (white_pawn_c_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score -= (white_pawn_d_file > 1) ? (white_pawn_d_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score -= (white_pawn_e_file > 1) ? (white_pawn_e_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score -= (white_pawn_f_file > 1) ? (white_pawn_f_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score -= (white_pawn_g_file > 1) ? (white_pawn_g_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score -= (white_pawn_h_file > 1) ? (white_pawn_h_file - 1) * DOUBLE_PAWN_PENALTY : 0;

    score += (black_pawn_a_file > 1) ? (black_pawn_a_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score += (black_pawn_b_file > 1) ? (black_pawn_b_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score += (black_pawn_c_file > 1) ? (black_pawn_c_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score += (black_pawn_d_file > 1) ? (black_pawn_d_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score += (black_pawn_e_file > 1) ? (black_pawn_e_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score += (black_pawn_f_file > 1) ? (black_pawn_f_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score += (black_pawn_g_file > 1) ? (black_pawn_g_file - 1) * DOUBLE_PAWN_PENALTY : 0;
    score += (black_pawn_h_file > 1) ? (black_pawn_h_file - 1) * DOUBLE_PAWN_PENALTY : 0;

    return score;
}

int32_t get_isolated_pawn_penalty(BoardState *board_state)
{
    return 0;
}

int32_t get_backward_pawn_penalty(BoardState *board_state)
{
    return 0;
}

int32_t get_passed_pawn_bonus(BoardState *board_state)
{
    return 0;
}

int32_t get_pawn_island_penalty(BoardState *board_state)
{
    return 0;
}

int32_t get_pawn_chain_bonus(BoardState *board_state)
{
    int32_t score = 0;
    uint64_t white_pawn_attacks = board_state->white_attacks.pawns;
    uint64_t black_pawn_attacks = board_state->black_attacks.pawns;

    score += __builtin_popcountll(board_state->board.white_pieces.pawns & white_pawn_attacks) * PAWN_CHAIN_BONUS;
    score -= __builtin_popcountll(board_state->board.black_pieces.pawns & black_pawn_attacks) * PAWN_CHAIN_BONUS;

    return score;
}

int32_t get_pawn_structure_score(BoardState *board_state)
{
    int32_t score = 0;
    score += get_double_pawn_penalty(board_state);
    score += get_isolated_pawn_penalty(board_state);
    score += get_backward_pawn_penalty(board_state);
    score += get_passed_pawn_bonus(board_state);
    score += get_pawn_island_penalty(board_state);
    score += get_pawn_chain_bonus(board_state);
    return score;
}