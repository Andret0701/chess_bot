#include "square_control.h"
#include "../../engine/attack_generation/attack_generation.h"
#include "heuristic_values.h"
#include "../../utils/bitboard.h"

double get_square_control(BoardState *board_state, double game_phase)
{
    double score = 0;

    uint64_t white_bishops = board_state->board.white_pieces.bishops;
    while (white_bishops)
    {
        int square = __builtin_ctzll(white_bishops);
        uint64_t attacks = generate_bishop_attack(board_state->occupied, square) & ~board_state->white_pieces;
        uint64_t my_side_attacks = attacks & WHITE_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & BLACK_SIDE_MASK;
        score += __builtin_popcountll(my_side_attacks) * ((1 - game_phase) * BISHOP_ATTACKS_MY_SIDE_MIDDLEGAME + game_phase * BISHOP_ATTACKS_MY_SIDE_ENDGAME);
        score += __builtin_popcountll(opponent_side_attacks) * ((1 - game_phase) * BISHOP_ATTACKS_OPPONENT_SIDE_MIDDLEGAME + game_phase * BISHOP_ATTACKS_OPPONENT_SIDE_ENDGAME);
        white_bishops &= white_bishops - 1;
    }

    uint64_t black_bishops = board_state->board.black_pieces.bishops;
    while (black_bishops)
    {
        int square = __builtin_ctzll(black_bishops);
        uint64_t attacks = generate_bishop_attack(board_state->occupied, square) & ~board_state->black_pieces;
        uint64_t my_side_attacks = attacks & BLACK_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & WHITE_SIDE_MASK;
        score -= __builtin_popcountll(my_side_attacks) * ((1 - game_phase) * BISHOP_ATTACKS_MY_SIDE_MIDDLEGAME + game_phase * BISHOP_ATTACKS_MY_SIDE_ENDGAME);
        score -= __builtin_popcountll(opponent_side_attacks) * ((1 - game_phase) * BISHOP_ATTACKS_OPPONENT_SIDE_MIDDLEGAME + game_phase * BISHOP_ATTACKS_OPPONENT_SIDE_ENDGAME);
        black_bishops &= black_bishops - 1;
    }

    uint64_t white_rooks = board_state->board.white_pieces.rooks;
    while (white_rooks)
    {
        int square = __builtin_ctzll(white_rooks);
        uint64_t attacks = generate_rook_attack(board_state->occupied, square) & ~board_state->white_pieces;
        uint64_t my_side_attacks = attacks & WHITE_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & BLACK_SIDE_MASK;
        score += __builtin_popcountll(my_side_attacks) * ((1 - game_phase) * ROOK_ATTACKS_MY_SIDE_MIDDLEGAME + game_phase * ROOK_ATTACKS_MY_SIDE_ENDGAME);
        score += __builtin_popcountll(opponent_side_attacks) * ((1 - game_phase) * ROOK_ATTACKS_OPPONENT_SIDE_MIDDLEGAME + game_phase * ROOK_ATTACKS_OPPONENT_SIDE_ENDGAME);
        white_rooks &= white_rooks - 1;
    }

    uint64_t black_rooks = board_state->board.black_pieces.rooks;
    while (black_rooks)
    {
        int square = __builtin_ctzll(black_rooks);
        uint64_t attacks = generate_rook_attack(board_state->occupied, square) & ~board_state->black_pieces;
        uint64_t my_side_attacks = attacks & BLACK_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & WHITE_SIDE_MASK;
        score -= __builtin_popcountll(my_side_attacks) * ((1 - game_phase) * ROOK_ATTACKS_MY_SIDE_MIDDLEGAME + game_phase * ROOK_ATTACKS_MY_SIDE_ENDGAME);
        score -= __builtin_popcountll(opponent_side_attacks) * ((1 - game_phase) * ROOK_ATTACKS_OPPONENT_SIDE_MIDDLEGAME + game_phase * ROOK_ATTACKS_OPPONENT_SIDE_ENDGAME);
        black_rooks &= black_rooks - 1;
    }

    uint64_t white_queens = board_state->board.white_pieces.queens;
    while (white_queens)
    {
        int square = __builtin_ctzll(white_queens);
        uint64_t attacks = generate_queen_attack(board_state->occupied, square) & ~board_state->white_pieces;
        uint64_t my_side_attacks = attacks & WHITE_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & BLACK_SIDE_MASK;
        score += __builtin_popcountll(my_side_attacks) * ((1 - game_phase) * QUEEN_ATTACKS_MY_SIDE_MIDDLEGAME + game_phase * QUEEN_ATTACKS_MY_SIDE_ENDGAME);
        score += __builtin_popcountll(opponent_side_attacks) * ((1 - game_phase) * QUEEN_ATTACKS_OPPONENT_SIDE_MIDDLEGAME + game_phase * QUEEN_ATTACKS_OPPONENT_SIDE_ENDGAME);
        white_queens &= white_queens - 1;
    }

    uint64_t black_queens = board_state->board.black_pieces.queens;
    while (black_queens)
    {
        int square = __builtin_ctzll(black_queens);
        uint64_t attacks = generate_queen_attack(board_state->occupied, square) & ~board_state->black_pieces;
        uint64_t my_side_attacks = attacks & BLACK_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & WHITE_SIDE_MASK;
        score -= __builtin_popcountll(my_side_attacks) * ((1 - game_phase) * QUEEN_ATTACKS_MY_SIDE_MIDDLEGAME + game_phase * QUEEN_ATTACKS_MY_SIDE_ENDGAME);
        score -= __builtin_popcountll(opponent_side_attacks) * ((1 - game_phase) * QUEEN_ATTACKS_OPPONENT_SIDE_MIDDLEGAME + game_phase * QUEEN_ATTACKS_OPPONENT_SIDE_ENDGAME);
        black_queens &= black_queens - 1;
    }

    return score;
}