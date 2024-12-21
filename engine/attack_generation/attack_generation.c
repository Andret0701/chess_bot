#include "attack_generation.h"

void generate_attacks(BoardState *board_state)
{
    board_state->black_attacks = (Pieces){0};
    board_state->white_attacks = (Pieces){0};
    
    for (uint8_t i = 0; i < 64; i++){
            uint64_t position = 1ULL << i;
            if ((board_state->occupied & position))
            {
                if (board_state->board.white_pieces.pawns & position)
                    board_state->white_attacks.pawns |= generate_white_pawn_attacks(i);
                else if (board_state->board.black_pieces.pawns & position)
                    board_state->black_attacks.pawns |= generate_black_pawn_attacks(i);
                else if (board_state->board.white_pieces.knights & position)
                    board_state->white_attacks.knights |= generate_knight_attacks(i);
                else if (board_state->board.black_pieces.knights & position)
                    board_state->black_attacks.knights |= generate_knight_attacks(i);
                else if (board_state->board.white_pieces.bishops & position)
                    board_state->white_attacks.bishops |= generate_bishop_attacks(board_state->occupied, i);
                else if (board_state->board.black_pieces.bishops & position)
                    board_state->black_attacks.bishops |= generate_bishop_attacks(board_state->occupied, i);
                else if (board_state->board.white_pieces.rooks & position)
                    board_state->white_attacks.rooks |= generate_rook_attacks(board_state->occupied, i);
                else if (board_state->board.black_pieces.rooks & position)
                    board_state->black_attacks.rooks |= generate_rook_attacks(board_state->occupied, i);
                else if (board_state->board.white_pieces.queens & position)
                    board_state->white_attacks.queens |= generate_queen_attacks(board_state->occupied, i);
                else if (board_state->board.black_pieces.queens & position)
                    board_state->black_attacks.queens |= generate_queen_attacks(board_state->occupied, i);
                else if (board_state->board.white_pieces.king & position)
                    board_state->white_attacks.king |= generate_king_attacks(i);
                else
                    board_state->black_attacks.king |= generate_king_attacks(i);
            }
    }

    board_state->white_attack = board_state->white_attacks.pawns | board_state->white_attacks.knights | board_state->white_attacks.bishops | board_state->white_attacks.rooks | board_state->white_attacks.queens | board_state->white_attacks.king;
    board_state->black_attack = board_state->black_attacks.pawns | board_state->black_attacks.knights | board_state->black_attacks.bishops | board_state->black_attacks.rooks | board_state->black_attacks.queens | board_state->black_attacks.king;
}