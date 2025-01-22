#include "../piece_moves.h"

bool white_pawn_advance_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if ((board_state->occupied & position_to_bitboard(x, y + 1)) == 0)
    {
        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x, y);
        new_board_state.board.white_pieces.pawns |= position_to_bitboard(x, y + 1);

        uint64_t attacks = generate_black_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.white_pieces.king))
            return true;

        if (y == 1)
        {
            if ((board_state->occupied & position_to_bitboard(x, y + 2)) == 0)
            {
                copy_board(&board_state->board, &new_board_state.board);
                new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x, y);
                new_board_state.board.white_pieces.pawns |= position_to_bitboard(x, y + 2);

                attacks = generate_black_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.white_pieces.king))
                    return true;
            }
        }
    }
    return false;
}

bool white_pawn_attack_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if (x > 0)
    {
        if ((board_state->black_pieces & position_to_bitboard(x - 1, y + 1)) != 0)
        {
            BoardState new_board_state = {0};
            copy_board(&board_state->board, &new_board_state.board);
            new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state.board.white_pieces.pawns |= position_to_bitboard(x - 1, y + 1);
            remove_black_piece(&new_board_state, x - 1, y + 1);

            uint64_t attacks = generate_black_attacks(&new_board_state.board);
            if (!(attacks & new_board_state.board.white_pieces.king))
                return true;
        }
    }

    if (x < 7)
    {
        if ((board_state->black_pieces & position_to_bitboard(x + 1, y + 1)) != 0)
        {
            BoardState new_board_state = {0};
            copy_board(&board_state->board, &new_board_state.board);
            new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state.board.white_pieces.pawns |= position_to_bitboard(x + 1, y + 1);
            remove_black_piece(&new_board_state, x + 1, y + 1);

            uint64_t attacks = generate_black_attacks(&new_board_state.board);
            if (!(attacks & new_board_state.board.white_pieces.king))
                return true;
        }
    }
    return false;
}

bool white_pawn_promote_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if (board_state->occupied & position_to_bitboard(x, y + 1))
        return false;

    BoardState new_board_state = {0};
    copy_board(&board_state->board, &new_board_state.board);
    new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x, y);
    new_board_state.board.white_pieces.knights |= position_to_bitboard(x, y + 1);

    uint64_t attacks = generate_black_attacks(&new_board_state.board);
    if (!(attacks & new_board_state.board.white_pieces.king))
        return true;

    return false;
}

bool white_pawn_promotion_attack_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if (x > 0 && (board_state->black_pieces & position_to_bitboard(x - 1, y + 1)) != 0)
    {
        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x, y);
        new_board_state.board.white_pieces.knights |= position_to_bitboard(x - 1, y + 1);
        remove_black_piece(&new_board_state, x - 1, y + 1);

        uint64_t attacks = generate_black_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.white_pieces.king))
            return true;
    }

    if (x < 7 && (board_state->black_pieces & position_to_bitboard(x + 1, y + 1)) != 0)
    {
        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x, y);
        new_board_state.board.white_pieces.knights |= position_to_bitboard(x + 1, y + 1);
        remove_black_piece(&new_board_state, x + 1, y + 1);

        uint64_t attacks = generate_black_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.white_pieces.king))
            return true;
    }

    return false;
}

bool white_pawn_en_passant_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if (y == 4)
    {
        if (x > 0)
        {
            if ((board_state->board.en_passant & position_to_bitboard(x - 1, y + 1)) != 0)
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x, y);
                new_board_state.board.white_pieces.pawns |= position_to_bitboard(x - 1, y + 1);
                new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x - 1, y);

                uint64_t attacks = generate_black_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.white_pieces.king))
                    return true;
            }
        }

        if (x < 7)
        {
            if ((board_state->board.en_passant & position_to_bitboard(x + 1, y + 1)) != 0)
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x, y);
                new_board_state.board.white_pieces.pawns |= position_to_bitboard(x + 1, y + 1);
                new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x + 1, y);

                uint64_t attacks = generate_black_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.white_pieces.king))
                    return true;
            }
        }
    }
    return false;
}

bool white_pawn_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if (y == 6)
    {
        if (white_pawn_promote_can_move(board_state, x, y))
            return true;
        if (white_pawn_promotion_attack_can_move(board_state, x, y))
            return true;
        return false;
    }

    if (white_pawn_advance_can_move(board_state, x, y))
        return true;
    if (white_pawn_attack_can_move(board_state, x, y))
        return true;
    if (white_pawn_en_passant_can_move(board_state, x, y))
        return true;

    return false;
}