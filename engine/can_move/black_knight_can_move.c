#include "../piece_moves.h"

bool black_knight_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    BoardState new_board_state = {0};
    static const int knight_moves[8][2] = {
        {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

    for (int i = 0; i < 8; ++i)
    {
        int new_x = x + knight_moves[i][0];
        int new_y = y + knight_moves[i][1];

        if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
        {
            if ((board_state->black_pieces & position_to_bitboard(new_x, new_y)) == 0)
            {
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, new_x, new_y);
                new_board_state.board.black_pieces.knights &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.knights |= position_to_bitboard(new_x, new_y);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
        }
    }

    return false;
}
