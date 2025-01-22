#include "../piece_moves.h"

bool white_king_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    BoardState new_board_state = {0};
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
                if (board_state->white_pieces & position_to_bitboard(new_x, new_y))
                    continue;

                copy_board(&board_state->board, &new_board_state.board);
                remove_black_piece(&new_board_state, new_x, new_y);

                new_board_state.board.white_pieces.king &= ~position_to_bitboard(x, y);
                new_board_state.board.white_pieces.king |= position_to_bitboard(new_x, new_y);

                uint64_t attacks = generate_black_attacks(&new_board_state.board);
                if ((attacks & new_board_state.board.white_pieces.king) == 0)
                    return true;
            }
        }
    }

    return false;
}