#include "../piece_moves.h"

bool white_rook_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    BoardState new_board_state = {0};

    // Right
    for (uint8_t i = x + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_u64(i, y))
        {
            if (board_state->black_pieces & position_to_u64(i, y))
            {
                copy_board(&board_state->board, &new_board_state.board);
                remove_black_piece(&new_board_state, i, y);
                new_board_state.board.white_pieces.rooks &= ~position_to_u64(x, y);
                new_board_state.board.white_pieces.rooks |= position_to_u64(i, y);

                uint64_t attacks = generate_black_attacks(&new_board_state);
                if (!(attacks & new_board_state.board.white_pieces.king))
                    return true;
            }
            break;
        }

        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.rooks &= ~position_to_u64(x, y);
        new_board_state.board.white_pieces.rooks |= position_to_u64(i, y);

        uint64_t attacks = generate_black_attacks(&new_board_state);
        if (!(attacks & new_board_state.board.white_pieces.king))
            return true;
    }

    // Left
    for (int8_t i = x - 1; i >= 0; i--)
    {
        if (board_state->occupied & position_to_u64(i, y))
        {
            if (board_state->black_pieces & position_to_u64(i, y))
            {
                copy_board(&board_state->board, &new_board_state.board);
                remove_black_piece(&new_board_state, i, y);
                new_board_state.board.white_pieces.rooks &= ~position_to_u64(x, y);
                new_board_state.board.white_pieces.rooks |= position_to_u64(i, y);

                uint64_t attacks = generate_black_attacks(&new_board_state);
                if (!(attacks & new_board_state.board.white_pieces.king))
                    return true;
            }
            break;
        }

        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.rooks &= ~position_to_u64(x, y);
        new_board_state.board.white_pieces.rooks |= position_to_u64(i, y);

        uint64_t attacks = generate_black_attacks(&new_board_state);
        if (!(attacks & new_board_state.board.white_pieces.king))
            return true;
    }

    // Up
    for (uint8_t i = y + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_u64(x, i))
        {
            if (board_state->black_pieces & position_to_u64(x, i))
            {
                copy_board(&board_state->board, &new_board_state.board);
                remove_black_piece(&new_board_state, x, i);
                new_board_state.board.white_pieces.rooks &= ~position_to_u64(x, y);
                new_board_state.board.white_pieces.rooks |= position_to_u64(x, i);

                uint64_t attacks = generate_black_attacks(&new_board_state);
                if (!(attacks & new_board_state.board.white_pieces.king))
                    return true;
            }
            break;
        }

        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.rooks &= ~position_to_u64(x, y);
        new_board_state.board.white_pieces.rooks |= position_to_u64(x, i);

        uint64_t attacks = generate_black_attacks(&new_board_state);
        if (!(attacks & new_board_state.board.white_pieces.king))
            return true;
    }

    // Down
    for (int8_t i = y - 1; i >= 0; i--)
    {
        if (board_state->occupied & position_to_u64(x, i))
        {
            if (board_state->black_pieces & position_to_u64(x, i))
            {
                copy_board(&board_state->board, &new_board_state.board);
                remove_black_piece(&new_board_state, x, i);
                new_board_state.board.white_pieces.rooks &= ~position_to_u64(x, y);
                new_board_state.board.white_pieces.rooks |= position_to_u64(x, i);

                uint64_t attacks = generate_black_attacks(&new_board_state);
                if (!(attacks & new_board_state.board.white_pieces.king))
                    return true;
            }
            break;
        }

        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.rooks &= ~position_to_u64(x, y);
        new_board_state.board.white_pieces.rooks |= position_to_u64(x, i);

        uint64_t attacks = generate_black_attacks(&new_board_state);
        if (!(attacks & new_board_state.board.white_pieces.king))
            return true;
    }

    return false;
}