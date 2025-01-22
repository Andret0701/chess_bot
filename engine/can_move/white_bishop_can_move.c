#include "../piece_moves.h"

bool white_bishop_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    BoardState new_board_state = {0};

    // NE direction
    for (uint8_t i = 1; x + i < 8 && y + i < 8; i++)
    {
        uint64_t target_pos = position_to_bitboard(x + i, y + i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                // Capture move
                copy_board(&board_state->board, &new_board_state.board);
                remove_black_piece(&new_board_state, x + i, y + i);
                new_board_state.board.white_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state.board.white_pieces.bishops |= target_pos;

                uint64_t attacks = generate_black_attacks(&new_board_state.board);
                if ((attacks & new_board_state.board.white_pieces.king) == 0)
                    return true;
            }
            break;
        }

        // Normal move
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.bishops &= ~position_to_bitboard(x, y);
        new_board_state.board.white_pieces.bishops |= target_pos;

        uint64_t attacks = generate_black_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.white_pieces.king))
            return true;
    }

    // SE direction
    for (uint8_t i = 1; x + i < 8 && y - i >= 0; i++)
    {
        uint64_t target_pos = position_to_bitboard(x + i, y - i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                // Capture move
                copy_board(&board_state->board, &new_board_state.board);
                remove_black_piece(&new_board_state, x + i, y - i);
                new_board_state.board.white_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state.board.white_pieces.bishops |= target_pos;

                uint64_t attacks = generate_black_attacks(&new_board_state.board);
                if ((attacks & new_board_state.board.white_pieces.king) == 0)
                    return true;
            }
            break;
        }

        // Normal move
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.bishops &= ~position_to_bitboard(x, y);
        new_board_state.board.white_pieces.bishops |= target_pos;

        uint64_t attacks = generate_black_attacks(&new_board_state.board);
        if ((attacks & new_board_state.board.white_pieces.king) == 0)
            return true;
    }

    // SW direction
    for (uint8_t i = 1; x - i >= 0 && y - i >= 0; i++)
    {
        uint64_t target_pos = position_to_bitboard(x - i, y - i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                // Capture move
                copy_board(&board_state->board, &new_board_state.board);
                remove_black_piece(&new_board_state, x - i, y - i);
                new_board_state.board.white_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state.board.white_pieces.bishops |= target_pos;

                uint64_t attacks = generate_black_attacks(&new_board_state.board);
                if ((attacks & new_board_state.board.white_pieces.king) == 0)
                    return true;
            }
            break;
        }

        // Normal move
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.bishops &= ~position_to_bitboard(x, y);
        new_board_state.board.white_pieces.bishops |= target_pos;

        uint64_t attacks = generate_black_attacks(&new_board_state.board);
        if ((attacks & new_board_state.board.white_pieces.king) == 0)
            return true;
    }

    // NW direction
    for (uint8_t i = 1; x - i >= 0 && y + i < 8; i++)
    {
        uint64_t target_pos = position_to_bitboard(x - i, y + i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                // Capture move
                copy_board(&board_state->board, &new_board_state.board);
                remove_black_piece(&new_board_state, x - i, y + i);
                new_board_state.board.white_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state.board.white_pieces.bishops |= target_pos;

                uint64_t attacks = generate_black_attacks(&new_board_state.board);
                if ((attacks & new_board_state.board.white_pieces.king) == 0)
                    return true;
            }
            break;
        }

        // Normal move
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.bishops &= ~position_to_bitboard(x, y);
        new_board_state.board.white_pieces.bishops |= target_pos;

        uint64_t attacks = generate_black_attacks(&new_board_state.board);
        if ((attacks & new_board_state.board.white_pieces.king) == 0)
            return true;
    }

    return false;
}