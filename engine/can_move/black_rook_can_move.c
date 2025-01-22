#include "../piece_moves.h"

bool black_rook_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    for (uint8_t i = x + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(i, y))
        {
            if (board_state->white_pieces & position_to_bitboard(i, y))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, i, y);
                new_board_state.board.black_pieces.rooks &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.rooks |= position_to_bitboard(i, y);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }

            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.rooks &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.rooks |= position_to_bitboard(i, y);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    for (uint8_t i = x - 1; i < 8; i--)
    {
        if (board_state->occupied & position_to_bitboard(i, y))
        {
            if (board_state->white_pieces & position_to_bitboard(i, y))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, i, y);
                new_board_state.board.black_pieces.rooks &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.rooks |= position_to_bitboard(i, y);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }

            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.rooks &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.rooks |= position_to_bitboard(i, y);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    for (uint8_t i = y + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(x, i))
        {
            if (board_state->white_pieces & position_to_bitboard(x, i))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x, i);
                new_board_state.board.black_pieces.rooks &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.rooks |= position_to_bitboard(x, i);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }

            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.rooks &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.rooks |= position_to_bitboard(x, i);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    for (uint8_t i = y - 1; i < 8; i--)
    {
        if (board_state->occupied & position_to_bitboard(x, i))
        {
            if (board_state->white_pieces & position_to_bitboard(x, i))
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                remove_white_piece(&new_board_state, x, i);
                new_board_state.board.black_pieces.rooks &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.rooks |= position_to_bitboard(x, i);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }

            break;
        }

        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.rooks &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.rooks |= position_to_bitboard(x, i);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    return false;
}