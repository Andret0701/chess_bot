#include "../piece_moves.h"

bool black_pawn_advance_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    // One step forward
    if ((board_state->occupied & position_to_bitboard(x, y - 1)) == 0)
    {
        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.pawns |= position_to_bitboard(x, y - 1);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;

        // Two steps forward
        if (y == 6)
        {
            if ((board_state->occupied & position_to_bitboard(x, y - 2)) == 0)
            {
                copy_board(&board_state->board, &new_board_state.board);
                new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.pawns |= position_to_bitboard(x, y - 2);

                attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
        }
    }

    return false;
}

bool black_pawn_attack_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if (x > 0)
    {
        if ((board_state->white_pieces & position_to_bitboard(x - 1, y - 1)) != 0)
        {
            BoardState new_board_state = {0};
            copy_board(&board_state->board, &new_board_state.board);
            new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state.board.black_pieces.pawns |= position_to_bitboard(x - 1, y - 1);
            remove_white_piece(&new_board_state, x - 1, y - 1);

            uint64_t attacks = generate_white_attacks(&new_board_state.board);
            if (!(attacks & new_board_state.board.black_pieces.king))
                return true;
        }
    }

    if (x < 7)
    {
        if ((board_state->white_pieces & position_to_bitboard(x + 1, y - 1)) != 0)
        {
            BoardState new_board_state = {0};
            copy_board(&board_state->board, &new_board_state.board);
            new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state.board.black_pieces.pawns |= position_to_bitboard(x + 1, y - 1);
            remove_white_piece(&new_board_state, x + 1, y - 1);

            uint64_t attacks = generate_white_attacks(&new_board_state.board);
            if (!(attacks & new_board_state.board.black_pieces.king))
                return true;
        }
    }

    return false;
}

bool black_pawn_promote_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if (board_state->occupied & position_to_bitboard(x, y - 1))
        return false;

    // Promote to knight (Assumes that if it can promote to a knight, it can promote to any piece)
    BoardState new_board_state = {0};
    copy_board(&board_state->board, &new_board_state.board);
    new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x, y);
    new_board_state.board.black_pieces.knights |= position_to_bitboard(x, y - 1);

    uint64_t attacks = generate_white_attacks(&new_board_state.board);
    if (!(attacks & new_board_state.board.black_pieces.king))
        return true;

    return false;
}

bool black_pawn_promotion_attack_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if (x > 0 && (board_state->white_pieces & position_to_bitboard(x - 1, y - 1)) != 0)
    {
        // Promote to knight (Assumes that if it can promote to a knight, it can promote to any piece)
        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.knights |= position_to_bitboard(x - 1, y - 1);
        remove_white_piece(&new_board_state, x - 1, y - 1);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    if (x < 7 && (board_state->white_pieces & position_to_bitboard(x + 1, y - 1)) != 0)
    {
        // Promote to knight (Assumes that if it can promote to a knight, it can promote to any piece)
        BoardState new_board_state = {0};
        copy_board(&board_state->board, &new_board_state.board);
        new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x, y);
        new_board_state.board.black_pieces.knights |= position_to_bitboard(x + 1, y - 1);
        remove_white_piece(&new_board_state, x + 1, y - 1);

        uint64_t attacks = generate_white_attacks(&new_board_state.board);
        if (!(attacks & new_board_state.board.black_pieces.king))
            return true;
    }

    return false;
}

bool black_pawn_en_passant_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if (y == 3)
    {
        if (x > 0)
        {
            if ((board_state->board.en_passant & position_to_bitboard(x - 1, y - 1)) != 0)
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.pawns |= position_to_bitboard(x - 1, y - 1);
                new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x - 1, y);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
        }

        if (x < 7)
        {
            if ((board_state->board.en_passant & position_to_bitboard(x + 1, y - 1)) != 0)
            {
                BoardState new_board_state = {0};
                copy_board(&board_state->board, &new_board_state.board);
                new_board_state.board.black_pieces.pawns &= ~position_to_bitboard(x, y);
                new_board_state.board.black_pieces.pawns |= position_to_bitboard(x + 1, y - 1);
                new_board_state.board.white_pieces.pawns &= ~position_to_bitboard(x + 1, y);

                uint64_t attacks = generate_white_attacks(&new_board_state.board);
                if (!(attacks & new_board_state.board.black_pieces.king))
                    return true;
            }
        }
    }

    return false;
}

bool black_pawn_can_move(BoardState *board_state, uint8_t x, uint8_t y)
{
    if (y == 1)
    {
        if (black_pawn_promote_can_move(board_state, x, y))
            return true;
        if (black_pawn_promotion_attack_can_move(board_state, x, y))
            return true;
        return false;
    }

    if (black_pawn_advance_can_move(board_state, x, y))
        return true;

    if (black_pawn_attack_can_move(board_state, x, y))
        return true;

    if (black_pawn_en_passant_can_move(board_state, x, y))
        return true;

    return false;
}
