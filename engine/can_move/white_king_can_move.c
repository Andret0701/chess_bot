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
                if (board_state->white_pieces & position_to_u64(new_x, new_y))
                    continue;

                if (!(board_state->black_attack & position_to_u64(new_x, new_y)))
                    return true;
            }
        }
    }

    // Castling
    if (x == 4 && y == 0 && !board_state->white_check)
    {
        // Kingside castle
        if ((board_state->board.castling_rights & WHITE_KINGSIDE_CASTLE) &&
            !(board_state->occupied & position_to_u64(5, 0)) &&
            !(board_state->occupied & position_to_u64(6, 0)) &&
            !(board_state->black_attack & position_to_u64(5, 0)))
        {
            copy_board(&board_state->board, &new_board_state.board);
            new_board_state.board.white_pieces.king &= ~position_to_u64(4, 0);
            new_board_state.board.white_pieces.king |= position_to_u64(6, 0);
            new_board_state.board.white_pieces.rooks &= ~position_to_u64(7, 0);
            new_board_state.board.white_pieces.rooks |= position_to_u64(5, 0);

            uint64_t attacks = generate_black_attacks(&new_board_state);
            if (!(attacks & new_board_state.board.white_pieces.king))
                return true;
        }

        // Queenside castle
        if ((board_state->board.castling_rights & WHITE_QUEENSIDE_CASTLE) &&
            !(board_state->occupied & position_to_u64(3, 0)) &&
            !(board_state->occupied & position_to_u64(2, 0)) &&
            !(board_state->occupied & position_to_u64(1, 0)) &&
            !(board_state->black_attack & position_to_u64(3, 0)) &&
            !(board_state->black_attack & position_to_u64(2, 0)))
        {
            copy_board(&board_state->board, &new_board_state.board);
            new_board_state.board.white_pieces.king &= ~position_to_u64(4, 0);
            new_board_state.board.white_pieces.king |= position_to_u64(2, 0);
            new_board_state.board.white_pieces.rooks &= ~position_to_u64(0, 0);
            new_board_state.board.white_pieces.rooks |= position_to_u64(3, 0);

            uint64_t attacks = generate_black_attacks(&new_board_state);
            if (!(attacks & new_board_state.board.white_pieces.king))
                return true;
        }
    }

    return false;
}