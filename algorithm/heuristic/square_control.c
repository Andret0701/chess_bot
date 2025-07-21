#include "square_control.h"
#include "../../engine/attack_generation/attack_generation.h"
#include "heuristic_values.h"

double get_square_control(BoardState *board_state, double game_phase)
{
    double score = 0;

    uint64_t white_bishops = board_state->board.white_pieces.bishops;
    uint64_t white_bishop_attacks = generate_bishop_attacks(board_state->occupied, white_bishops);
    score += __builtin_popcountll(white_bishop_attacks) * ((1 - game_phase) * BISHOP_ATTACKS_MIDDLEGAME + game_phase * BISHOP_ATTACKS_ENDGAME);

    uint64_t black_bishops = board_state->board.black_pieces.bishops;
    uint64_t black_bishop_attacks = generate_bishop_attacks(board_state->occupied, black_bishops);
    score -= __builtin_popcountll(black_bishop_attacks) * ((1 - game_phase) * BISHOP_ATTACKS_MIDDLEGAME + game_phase * BISHOP_ATTACKS_ENDGAME);

    uint64_t white_rooks = board_state->board.white_pieces.rooks;
    uint64_t white_rook_attacks = generate_rook_attacks(board_state->occupied, white_rooks);
    score += __builtin_popcountll(white_rook_attacks) * ((1 - game_phase) * ROOK_ATTACKS_MIDDLEGAME + game_phase * ROOK_ATTACKS_ENDGAME);

    uint64_t black_rooks = board_state->board.black_pieces.rooks;
    uint64_t black_rook_attacks = generate_rook_attacks(board_state->occupied, black_rooks);
    score -= __builtin_popcountll(black_rook_attacks) * ((1 - game_phase) * ROOK_ATTACKS_MIDDLEGAME + game_phase * ROOK_ATTACKS_ENDGAME);

    uint64_t white_queens = board_state->board.white_pieces.queens;
    uint64_t white_queen_attacks = generate_queen_attacks(board_state->occupied, white_queens);
    score += __builtin_popcountll(white_queen_attacks) * ((1 - game_phase) * QUEEN_ATTACKS_MIDDLEGAME + game_phase * QUEEN_ATTACKS_ENDGAME);

    uint64_t black_queens = board_state->board.black_pieces.queens;
    uint64_t black_queen_attacks = generate_queen_attacks(board_state->occupied, black_queens);
    score -= __builtin_popcountll(black_queen_attacks) * ((1 - game_phase) * QUEEN_ATTACKS_MIDDLEGAME + game_phase * QUEEN_ATTACKS_ENDGAME);

    return score;
}