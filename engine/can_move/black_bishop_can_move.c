#include "../piece_moves.h"

bool black_bishop_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    BoardState new_board_state = {0};

    // Loop to check moves in the NE direction
    for (uint8_t i = 1; x + i < 8 && y + i < 8; i++)
    {
        uint64_t target_pos = position_to_bitboard(x + i, y + i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->white_pieces & target_pos)
            {
                // Capture move
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x + i, y + i);
                new_board_state.board.black_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.bishops |= target_pos;

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }

            break;
        }

        // Normal move
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.bishops &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.bishops |= target_pos;

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    // Loop to check moves in the SE direction
    for (uint8_t i = 1; x + i < 8 && y - i >= 0; i++)
    {
        uint64_t target_pos = position_to_bitboard(x + i, y - i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->white_pieces & target_pos)
            {
                // Capture move
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x + i, y - i);
                new_board_state.board.black_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.bishops |= target_pos;

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }

            break;
        }
        // Normal move
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.bishops &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.bishops |= target_pos;

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    // Loop to check moves in the SW direction
    for (uint8_t i = 1; x - i >= 0 && y + i < 8; i++)
    {
        uint64_t target_pos = position_to_bitboard(x - i, y + i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->white_pieces & target_pos)
            {
                // Capture move
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x - i, y + i);
                new_board_state.board.black_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.bishops |= target_pos;

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }

            break;
        }

        // Normal move
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.bishops &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.bishops |= target_pos;

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    // Loop to check moves in the NW direction
    for (uint8_t i = 1; x - i >= 0 && y - i >= 0; i++)
    {
        uint64_t target_pos = position_to_bitboard(x - i, y - i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->white_pieces & target_pos)
            {
                // Capture move
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x - i, y - i);
                new_board_state.board.black_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.bishops |= target_pos;

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }

            break;
        }

        // Normal move
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.bishops &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.bishops |= target_pos;

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    return false;
}
