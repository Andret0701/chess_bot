#include "square_control.h"
#include "../../engine/attack_generation/attack_generation.h"

int32_t get_square_control(BoardState *board_state)
{
    int32_t score = 0;

    // These are commented out as I think the piece square tables allready do this
    // uint64_t white_pawns = board_state->board.white_pieces.pawns;
    // while (white_pawns)
    // {
    //     int square = __builtin_ctzll(white_pawns);
    //     uint64_t position = 1ULL << square;
    //     uint64_t attacks = generate_white_pawn_attacks(position);
    //     score += __builtin_popcountll(attacks) * 9;
    //     white_pawns &= white_pawns - 1;
    // }

    // uint64_t black_pawns = board_state->board.black_pieces.pawns;
    // while (black_pawns)
    // {
    //     int square = __builtin_ctzll(black_pawns);
    //     uint64_t position = 1ULL << square;
    //     uint64_t attacks = generate_black_pawn_attacks(position);
    //     score -= __builtin_popcountll(attacks) * 9;
    //     black_pawns &= black_pawns - 1;
    // }

    // uint64_t white_knights = board_state->board.white_pieces.knights;
    // while (white_knights)
    // {
    //     int square = __builtin_ctzll(white_knights);
    //     uint64_t position = 1ULL << square;
    //     uint64_t attacks = generate_knight_attacks(position);
    //     score += __builtin_popcountll(attacks) * 6;
    //     white_knights &= white_knights - 1;
    // }

    // uint64_t black_knights = board_state->board.black_pieces.knights;
    // while (black_knights)
    // {
    //     int square = __builtin_ctzll(black_knights);
    //     uint64_t position = 1ULL << square;
    //     uint64_t attacks = generate_knight_attacks(position);
    //     score -= __builtin_popcountll(attacks) * 6;
    //     black_knights &= black_knights - 1;
    // }

    uint64_t white_bishops = board_state->board.white_pieces.bishops;
    while (white_bishops)
    {
        int square = __builtin_ctzll(white_bishops);
        uint64_t attacks = generate_bishop_attacks(board_state->occupied, square);
        score += __builtin_popcountll(attacks) * 5;
        white_bishops &= white_bishops - 1;
    }

    uint64_t black_bishops = board_state->board.black_pieces.bishops;
    while (black_bishops)
    {
        int square = __builtin_ctzll(black_bishops);
        uint64_t attacks = generate_bishop_attacks(board_state->occupied, square);
        score -= __builtin_popcountll(attacks) * 5;
        black_bishops &= black_bishops - 1;
    }

    uint64_t white_rooks = board_state->board.white_pieces.rooks;
    while (white_rooks)
    {
        int square = __builtin_ctzll(white_rooks);
        uint64_t attacks = generate_rook_attacks(board_state->occupied, square);
        score += __builtin_popcountll(attacks) * 2;
        white_rooks &= white_rooks - 1;
    }

    uint64_t black_rooks = board_state->board.black_pieces.rooks;
    while (black_rooks)
    {
        int square = __builtin_ctzll(black_rooks);
        uint64_t attacks = generate_rook_attacks(board_state->occupied, square);
        score -= __builtin_popcountll(attacks) * 2;
        black_rooks &= black_rooks - 1;
    }

    uint64_t white_queens = board_state->board.white_pieces.queens;
    while (white_queens)
    {
        int square = __builtin_ctzll(white_queens);
        uint64_t attacks = generate_queen_attacks(board_state->occupied, square);
        score += __builtin_popcountll(attacks) * 1;
        white_queens &= white_queens - 1;
    }

    uint64_t black_queens = board_state->board.black_pieces.queens;
    while (black_queens)
    {
        int square = __builtin_ctzll(black_queens);
        uint64_t attacks = generate_queen_attacks(board_state->occupied, square);
        score -= __builtin_popcountll(attacks) * 1;
        black_queens &= black_queens - 1;
    }

    return score;
}