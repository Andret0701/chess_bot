#include "../piece_moves.h"

void generate_white_king_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;

            int new_x = x + dx;
            int new_y = y + dy;

            if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
            {
                if (!(board_state->black_pieces & position_to_bitboard(new_x, new_y)))
                    continue;

                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, new_x, new_y);

                new_board_state->board.white_pieces.king &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.king |= position_to_bitboard(new_x, new_y);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                new_board_state->board.castling_rights &= ~WHITE_KINGSIDE_CASTLE;
                new_board_state->board.castling_rights &= ~WHITE_QUEENSIDE_CASTLE;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                validate_white_move(stack);
            }
        }
    }
}