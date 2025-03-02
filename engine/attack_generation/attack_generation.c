#include "attack_generation.h"

uint64_t generate_white_attacks(Board *board)
{
    uint64_t occupied = board->white_pieces.pawns | board->white_pieces.knights | board->white_pieces.bishops | board->white_pieces.rooks | board->white_pieces.queens | board->white_pieces.king | board->black_pieces.pawns | board->black_pieces.knights | board->black_pieces.bishops | board->black_pieces.rooks | board->black_pieces.queens | board->black_pieces.king;
    uint64_t attacks = generate_white_pawn_attacks(board->white_pieces.pawns);
    attacks |= generate_knight_attacks(board->white_pieces.knights);
    attacks |= generate_king_attacks(board->white_pieces.king);

    uint64_t white_bishops = board->white_pieces.bishops;
    while (white_bishops)
    {
        int index = __builtin_ctzll(white_bishops);
        white_bishops &= white_bishops - 1;
        attacks |= generate_bishop_attacks(occupied, index);
    }

    uint64_t white_rooks = board->white_pieces.rooks;
    while (white_rooks)
    {
        int index = __builtin_ctzll(white_rooks);
        white_rooks &= white_rooks - 1;
        attacks |= generate_rook_attacks(occupied, index);
    }

    uint64_t white_queens = board->white_pieces.queens;
    while (white_queens)
    {
        int index = __builtin_ctzll(white_queens);
        white_queens &= white_queens - 1;
        attacks |= generate_queen_attacks(occupied, index);
    }

    return attacks;
}

uint64_t generate_black_attacks(Board *board)
{
    uint64_t occupied = board->white_pieces.pawns | board->white_pieces.knights | board->white_pieces.bishops | board->white_pieces.rooks | board->white_pieces.queens | board->white_pieces.king | board->black_pieces.pawns | board->black_pieces.knights | board->black_pieces.bishops | board->black_pieces.rooks | board->black_pieces.queens | board->black_pieces.king;
    uint64_t attacks = generate_black_pawn_attacks(board->black_pieces.pawns);
    attacks |= generate_knight_attacks(board->black_pieces.knights);
    attacks |= generate_king_attacks(board->black_pieces.king);

    uint64_t black_bishops = board->black_pieces.bishops;
    while (black_bishops)
    {
        int index = __builtin_ctzll(black_bishops);
        black_bishops &= black_bishops - 1;
        attacks |= generate_bishop_attacks(occupied, index);
    }

    uint64_t black_rooks = board->black_pieces.rooks;
    while (black_rooks)
    {
        int index = __builtin_ctzll(black_rooks);
        black_rooks &= black_rooks - 1;
        attacks |= generate_rook_attacks(occupied, index);
    }

    uint64_t black_queens = board->black_pieces.queens;
    while (black_queens)
    {
        int index = __builtin_ctzll(black_queens);
        black_queens &= black_queens - 1;
        attacks |= generate_queen_attacks(occupied, index);
    }

    return attacks;
}

void generate_attacks(BoardState *board_state)
{
    //        Each sample counts as 0.01 seconds.
    //        %   cumulative   self              self     total
    //       time   seconds   seconds    calls   s/call   s/call  name
    //       0.99     71.99     0.73 253760712     0.00     0.00  generate_rook_attacks
    //       0.86     72.63     0.64 245310542     0.00     0.00  generate_bishop_attacks
    board_state->black_attacks = (Pieces){0};
    board_state->white_attacks = (Pieces){0};

    board_state->white_attacks.pawns = generate_white_pawn_attacks(board_state->board.white_pieces.pawns);
    board_state->black_attacks.pawns = generate_black_pawn_attacks(board_state->board.black_pieces.pawns);
    board_state->white_attacks.knights = generate_knight_attacks(board_state->board.white_pieces.knights);
    board_state->black_attacks.knights = generate_knight_attacks(board_state->board.black_pieces.knights);
    board_state->white_attacks.king = generate_king_attacks(board_state->board.white_pieces.king);
    board_state->black_attacks.king = generate_king_attacks(board_state->board.black_pieces.king);

    uint64_t occupied = board_state->occupied;

    // Process white bishops
    uint64_t white_bishops = board_state->board.white_pieces.bishops;
    while (white_bishops)
    {
        int index = __builtin_ctzll(white_bishops);
        white_bishops &= white_bishops - 1;
        board_state->white_attacks.bishops |= generate_bishop_attacks(occupied, index);
    }

    // Process white rooks
    uint64_t white_rooks = board_state->board.white_pieces.rooks;
    while (white_rooks)
    {
        int index = __builtin_ctzll(white_rooks);
        white_rooks &= white_rooks - 1;
        board_state->white_attacks.rooks |= generate_rook_attacks(occupied, index);
    }

    // Process white queens
    uint64_t white_queens = board_state->board.white_pieces.queens;
    while (white_queens)
    {
        int index = __builtin_ctzll(white_queens);
        white_queens &= white_queens - 1;
        board_state->white_attacks.queens |= generate_queen_attacks(occupied, index);
    }

    // Process black bishops
    uint64_t black_bishops = board_state->board.black_pieces.bishops;
    while (black_bishops)
    {
        int index = __builtin_ctzll(black_bishops);
        black_bishops &= black_bishops - 1;
        board_state->black_attacks.bishops |= generate_bishop_attacks(occupied, index);
    }

    // Process black rooks
    uint64_t black_rooks = board_state->board.black_pieces.rooks;
    while (black_rooks)
    {
        int index = __builtin_ctzll(black_rooks);
        black_rooks &= black_rooks - 1;
        board_state->black_attacks.rooks |= generate_rook_attacks(occupied, index);
    }

    // Process black queens
    uint64_t black_queens = board_state->board.black_pieces.queens;
    while (black_queens)
    {
        int index = __builtin_ctzll(black_queens);
        black_queens &= black_queens - 1;
        board_state->black_attacks.queens |= generate_queen_attacks(board_state->occupied, index);
    }

    board_state->white_attack = board_state->white_attacks.pawns | board_state->white_attacks.knights | board_state->white_attacks.bishops | board_state->white_attacks.rooks | board_state->white_attacks.queens | board_state->white_attacks.king;
    board_state->black_attack = board_state->black_attacks.pawns | board_state->black_attacks.knights | board_state->black_attacks.bishops | board_state->black_attacks.rooks | board_state->black_attacks.queens | board_state->black_attacks.king;
}