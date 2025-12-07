#include "square_control.h"
#include "../../engine/attack_generation/attack_generation.h"
#include "heuristic_values.h"
#include "../../utils/bitboard.h"

SquareControlFeatures get_square_control_features(BoardState *board_state, double game_phase)
{
    int8_t bishop_my_side_attacks_white = 0;
    int8_t bishop_opponent_side_attacks_white = 0;
    int8_t rook_my_side_attacks_white = 0;
    int8_t rook_opponent_side_attacks_white = 0;
    int8_t queen_my_side_attacks_white = 0;
    int8_t queen_opponent_side_attacks_white = 0;

    int8_t bishop_my_side_attacks_black = 0;
    int8_t bishop_opponent_side_attacks_black = 0;
    int8_t rook_my_side_attacks_black = 0;
    int8_t rook_opponent_side_attacks_black = 0;
    int8_t queen_my_side_attacks_black = 0;
    int8_t queen_opponent_side_attacks_black = 0;

    uint64_t white_bishops = board_state->board.white_pieces.bishops;
    while (white_bishops)
    {
        int square = __builtin_ctzll(white_bishops);
        uint64_t attacks = generate_bishop_attack(board_state->occupied, square) & ~board_state->white_pieces;
        uint64_t my_side_attacks = attacks & WHITE_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & BLACK_SIDE_MASK;
        bishop_my_side_attacks_white += __builtin_popcountll(my_side_attacks);
        bishop_opponent_side_attacks_white += __builtin_popcountll(opponent_side_attacks);
        white_bishops &= white_bishops - 1;
    }

    uint64_t black_bishops = board_state->board.black_pieces.bishops;
    while (black_bishops)
    {
        int square = __builtin_ctzll(black_bishops);
        uint64_t attacks = generate_bishop_attack(board_state->occupied, square) & ~board_state->black_pieces;
        uint64_t my_side_attacks = attacks & BLACK_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & WHITE_SIDE_MASK;
        bishop_my_side_attacks_black += __builtin_popcountll(my_side_attacks);
        bishop_opponent_side_attacks_black += __builtin_popcountll(opponent_side_attacks);
        black_bishops &= black_bishops - 1;
    }

    uint64_t white_rooks = board_state->board.white_pieces.rooks;
    while (white_rooks)
    {
        int square = __builtin_ctzll(white_rooks);
        uint64_t attacks = generate_rook_attack(board_state->occupied, square) & ~board_state->white_pieces;
        uint64_t my_side_attacks = attacks & WHITE_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & BLACK_SIDE_MASK;
        rook_my_side_attacks_white += __builtin_popcountll(my_side_attacks);
        rook_opponent_side_attacks_white += __builtin_popcountll(opponent_side_attacks);
        white_rooks &= white_rooks - 1;
    }

    uint64_t black_rooks = board_state->board.black_pieces.rooks;
    while (black_rooks)
    {
        int square = __builtin_ctzll(black_rooks);
        uint64_t attacks = generate_rook_attack(board_state->occupied, square) & ~board_state->black_pieces;
        uint64_t my_side_attacks = attacks & BLACK_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & WHITE_SIDE_MASK;
        rook_my_side_attacks_black += __builtin_popcountll(my_side_attacks);
        rook_opponent_side_attacks_black += __builtin_popcountll(opponent_side_attacks);
        black_rooks &= black_rooks - 1;
    }

    uint64_t white_queens = board_state->board.white_pieces.queens;
    while (white_queens)
    {
        int square = __builtin_ctzll(white_queens);
        uint64_t attacks = generate_queen_attack(board_state->occupied, square) & ~board_state->white_pieces;
        uint64_t my_side_attacks = attacks & WHITE_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & BLACK_SIDE_MASK;
        queen_my_side_attacks_white += __builtin_popcountll(my_side_attacks);
        queen_opponent_side_attacks_white += __builtin_popcountll(opponent_side_attacks);
        white_queens &= white_queens - 1;
    }

    uint64_t black_queens = board_state->board.black_pieces.queens;
    while (black_queens)
    {
        int square = __builtin_ctzll(black_queens);
        uint64_t attacks = generate_queen_attack(board_state->occupied, square) & ~board_state->black_pieces;
        uint64_t my_side_attacks = attacks & BLACK_SIDE_MASK;
        uint64_t opponent_side_attacks = attacks & WHITE_SIDE_MASK;
        queen_my_side_attacks_black += __builtin_popcountll(my_side_attacks);
        queen_opponent_side_attacks_black += __builtin_popcountll(opponent_side_attacks);
        black_queens &= black_queens - 1;
    }

    return (SquareControlFeatures){
        create_feature(bishop_my_side_attacks_white, bishop_my_side_attacks_black, game_phase),
        create_feature(bishop_opponent_side_attacks_white, bishop_opponent_side_attacks_black, game_phase),
        create_feature(rook_my_side_attacks_white, rook_my_side_attacks_black, game_phase),
        create_feature(rook_opponent_side_attacks_white, rook_opponent_side_attacks_black, game_phase),
        create_feature(queen_my_side_attacks_white, queen_my_side_attacks_black, game_phase),
        create_feature(queen_opponent_side_attacks_white, queen_opponent_side_attacks_black, game_phase)};
}