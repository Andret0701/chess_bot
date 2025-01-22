#include "../piece_moves.h"

bool black_queen_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    // Right
    for (uint8_t i = x + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(i, y))
        {
            if (board_state->white_pieces & position_to_bitboard(i, y))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, i, y);
                new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.queens |= position_to_bitboard(i, y);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.queens |= position_to_bitboard(i, y);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    // Left
    for (int8_t i = x - 1; i >= 0; i--)
    {
        if (board_state->occupied & position_to_bitboard(i, y))
        {
            if (board_state->white_pieces & position_to_bitboard(i, y))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, i, y);
                new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.queens |= position_to_bitboard(i, y);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.queens |= position_to_bitboard(i, y);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    // Up
    for (uint8_t i = y + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(x, i))
        {
            if (board_state->white_pieces & position_to_bitboard(x, i))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x, i);
                new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.queens |= position_to_bitboard(x, i);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.queens |= position_to_bitboard(x, i);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    // Down
    for (int8_t i = y - 1; i >= 0; i--)
    {
        if (board_state->occupied & position_to_bitboard(x, i))
        {
            if (board_state->white_pieces & position_to_bitboard(x, i))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x, i);
                new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.queens |= position_to_bitboard(x, i);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.queens |= position_to_bitboard(x, i);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    // Diagonal movements
    // Up-Right
    for (uint8_t i = 1; x + i < 8 && y + i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(x + i, y + i))
        {
            if (board_state->white_pieces & position_to_bitboard(x + i, y + i))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x + i, y + i);
                new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.queens |= position_to_bitboard(x + i, y + i);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.queens |= position_to_bitboard(x + i, y + i);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    // Up-Left
    for (uint8_t i = 1; x - i >= 0 && y + i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(x - i, y + i))
        {
            if (board_state->white_pieces & position_to_bitboard(x - i, y + i))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x - i, y + i);
                new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.queens |= position_to_bitboard(x - i, y + i);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.queens |= position_to_bitboard(x - i, y + i);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    // Down-Right
    for (uint8_t i = 1; x + i < 8 && y - i >= 0; i++)
    {
        if (board_state->occupied & position_to_bitboard(x + i, y - i))
        {
            if (board_state->white_pieces & position_to_bitboard(x + i, y - i))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x + i, y - i);
                new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.queens |= position_to_bitboard(x + i, y - i);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.queens |= position_to_bitboard(x + i, y - i);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    // Down-Left
    for (uint8_t i = 1; x - i >= 0 && y - i >= 0; i++)
    {
        if (board_state->occupied & position_to_bitboard(x - i, y - i))
        {
            if (board_state->white_pieces & position_to_bitboard(x - i, y - i))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x - i, y - i);
                new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.queens |= position_to_bitboard(x - i, y - i);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.queens |= position_to_bitboard(x - i, y - i);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    return false;
}