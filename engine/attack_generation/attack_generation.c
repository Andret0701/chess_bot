#include "attack_generation.h"

uint64_t generate_white_attacks_mask(Board *board)
{
    uint64_t occupied = board->white_pieces.pawns | board->white_pieces.knights | board->white_pieces.bishops | board->white_pieces.rooks | board->white_pieces.queens | board->white_pieces.king | board->black_pieces.pawns | board->black_pieces.knights | board->black_pieces.bishops | board->black_pieces.rooks | board->black_pieces.queens | board->black_pieces.king;
    uint64_t attacks = generate_white_pawn_attacks(board->white_pieces.pawns);
    attacks |= generate_knight_attacks(board->white_pieces.knights);
    attacks |= generate_king_attacks(board->white_pieces.king);

    attacks |= generate_bishop_attacks(occupied, board->white_pieces.bishops);
    attacks |= generate_rook_attacks(occupied, board->white_pieces.rooks);
    attacks |= generate_queen_attacks(occupied, board->white_pieces.queens);

    return attacks;
}

uint64_t generate_black_attacks_mask(Board *board)
{
    uint64_t occupied = board->white_pieces.pawns | board->white_pieces.knights | board->white_pieces.bishops | board->white_pieces.rooks | board->white_pieces.queens | board->white_pieces.king | board->black_pieces.pawns | board->black_pieces.knights | board->black_pieces.bishops | board->black_pieces.rooks | board->black_pieces.queens | board->black_pieces.king;
    uint64_t attacks = generate_black_pawn_attacks(board->black_pieces.pawns);
    attacks |= generate_knight_attacks(board->black_pieces.knights);
    attacks |= generate_king_attacks(board->black_pieces.king);

    attacks |= generate_bishop_attacks(occupied, board->black_pieces.bishops);
    attacks |= generate_rook_attacks(occupied, board->black_pieces.rooks);
    attacks |= generate_queen_attacks(occupied, board->black_pieces.queens);

    return attacks;
}

void generate_white_attacks(const BoardState *from, BoardState *to)
{
    uint64_t occupied = to->occupied;
    uint64_t occupied_delta = from->occupied ^ to->occupied;

    to->white_attacks = (Pieces){0};
    if (to->board.white_pieces.pawns != from->board.white_pieces.pawns)
        to->white_attacks.pawns = generate_white_pawn_attacks(to->board.white_pieces.pawns);
    else
        to->white_attacks.pawns = from->white_attacks.pawns;
    if (to->board.white_pieces.knights != from->board.white_pieces.knights)
        to->white_attacks.knights = generate_knight_attacks(to->board.white_pieces.knights);
    else
        to->white_attacks.knights = from->white_attacks.knights;
    if (to->board.white_pieces.king != from->board.white_pieces.king)
        to->white_attacks.king = generate_king_attacks(to->board.white_pieces.king);
    else
        to->white_attacks.king = from->white_attacks.king;

    if (to->board.white_pieces.bishops != from->board.white_pieces.bishops || occupied_delta & from->white_attacks.bishops)
        to->white_attacks.bishops = generate_bishop_attacks(occupied, to->board.white_pieces.bishops);
    else
        to->white_attacks.bishops = from->white_attacks.bishops;
    if (to->board.white_pieces.rooks != from->board.white_pieces.rooks || occupied_delta & from->white_attacks.rooks)
        to->white_attacks.rooks = generate_rook_attacks(occupied, to->board.white_pieces.rooks);
    else
        to->white_attacks.rooks = from->white_attacks.rooks;
    if (to->board.white_pieces.queens != from->board.white_pieces.queens || occupied_delta & from->white_attacks.queens)
        to->white_attacks.queens = generate_queen_attacks(occupied, to->board.white_pieces.queens);
    else
        to->white_attacks.queens = from->white_attacks.queens;

    to->white_attack = to->white_attacks.pawns | to->white_attacks.knights | to->white_attacks.bishops | to->white_attacks.rooks | to->white_attacks.queens | to->white_attacks.king;
}

void generate_black_attacks(const BoardState *from, BoardState *to)
{
    uint64_t occupied = to->occupied;
    uint64_t occupied_delta = from->occupied ^ to->occupied;

    to->black_attacks = (Pieces){0};
    if (to->board.black_pieces.pawns != from->board.black_pieces.pawns)
        to->black_attacks.pawns = generate_black_pawn_attacks(to->board.black_pieces.pawns);
    else
        to->black_attacks.pawns = from->black_attacks.pawns;
    if (to->board.black_pieces.knights != from->board.black_pieces.knights)
        to->black_attacks.knights = generate_knight_attacks(to->board.black_pieces.knights);
    else
        to->black_attacks.knights = from->black_attacks.knights;
    if (to->board.black_pieces.king != from->board.black_pieces.king)
        to->black_attacks.king = generate_king_attacks(to->board.black_pieces.king);
    else
        to->black_attacks.king = from->black_attacks.king;

    if (to->board.black_pieces.bishops != from->board.black_pieces.bishops || occupied_delta & from->black_attacks.bishops)
        to->black_attacks.bishops = generate_bishop_attacks(occupied, to->board.black_pieces.bishops);
    else
        to->black_attacks.bishops = from->black_attacks.bishops;
    if (to->board.black_pieces.rooks != from->board.black_pieces.rooks || occupied_delta & from->black_attacks.rooks)
        to->black_attacks.rooks = generate_rook_attacks(occupied, to->board.black_pieces.rooks);
    else
        to->black_attacks.rooks = from->black_attacks.rooks;
    if (to->board.black_pieces.queens != from->board.black_pieces.queens || occupied_delta & from->black_attacks.queens)
        to->black_attacks.queens = generate_queen_attacks(occupied, to->board.black_pieces.queens);
    else
        to->black_attacks.queens = from->black_attacks.queens;

    to->black_attack = to->black_attacks.pawns | to->black_attacks.knights | to->black_attacks.bishops | to->black_attacks.rooks | to->black_attacks.queens | to->black_attacks.king;
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

    board_state->white_attacks.bishops = generate_bishop_attacks(occupied, board_state->board.white_pieces.bishops);
    board_state->black_attacks.bishops = generate_bishop_attacks(occupied, board_state->board.black_pieces.bishops);
    board_state->white_attacks.rooks = generate_rook_attacks(occupied, board_state->board.white_pieces.rooks);
    board_state->black_attacks.rooks = generate_rook_attacks(occupied, board_state->board.black_pieces.rooks);
    board_state->white_attacks.queens = generate_queen_attacks(occupied, board_state->board.white_pieces.queens);
    board_state->black_attacks.queens = generate_queen_attacks(occupied, board_state->board.black_pieces.queens);

    board_state->white_attack = board_state->white_attacks.pawns | board_state->white_attacks.knights | board_state->white_attacks.bishops | board_state->white_attacks.rooks | board_state->white_attacks.queens | board_state->white_attacks.king;
    board_state->black_attack = board_state->black_attacks.pawns | board_state->black_attacks.knights | board_state->black_attacks.bishops | board_state->black_attacks.rooks | board_state->black_attacks.queens | board_state->black_attacks.king;
}