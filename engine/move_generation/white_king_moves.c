#include "../piece_moves.h"

void generate_white_king_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
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

                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, new_x, new_y);

                new_board_state->board.white_pieces.king &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.king |= position_to_bitboard(new_x, new_y);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                new_board_state->board.castling_rights &= ~WHITE_KINGSIDE_CASTLE;
                new_board_state->board.castling_rights &= ~WHITE_QUEENSIDE_CASTLE;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(new_x, new_y), is_black_piece(board_state, new_x, new_y));
                validate_white_move(stack);
            }
        }
    }

    // Castling
    if (x == 4 && y == 0)
    {
        if (!board_state->white_check)
        {

            if ((board_state->board.castling_rights & WHITE_KINGSIDE_CASTLE) != 0)
            {
                if ((board_state->occupied & position_to_bitboard(5, 0)) == 0 &&
                    (board_state->occupied & position_to_bitboard(6, 0)) == 0 &&
                    (board_state->black_attack & position_to_bitboard(5, 0)) == 0)
                {
                    BoardState *new_board_state = &stack->boards[stack->count];
                    copy_board(&board_state->board, &new_board_state->board);

                    new_board_state->board.white_pieces.king &= ~position_to_bitboard(4, 0);
                    new_board_state->board.white_pieces.king |= position_to_bitboard(6, 0);
                    new_board_state->board.white_pieces.rooks &= ~position_to_bitboard(7, 0);
                    new_board_state->board.white_pieces.rooks |= position_to_bitboard(5, 0);
                    new_board_state->board.side_to_move = BLACK;
                    new_board_state->board.en_passant = 0;
                    new_board_state->board.castling_rights &= ~WHITE_KINGSIDE_CASTLE;
                    new_board_state->board.castling_rights &= ~WHITE_QUEENSIDE_CASTLE;
                    init_board(new_board_state);
                    new_board_state->has_castled = board_state->has_castled | WHITE_KINGSIDE_CASTLE;
                    new_board_state->move = new_castling_encoded_move(position_to_index(x, y), position_to_index(6, 0));
                    validate_white_move(stack);
                }
            }

            if ((board_state->board.castling_rights & WHITE_QUEENSIDE_CASTLE) != 0)
            {
                if ((board_state->occupied & position_to_bitboard(3, 0)) == 0 &&
                    (board_state->occupied & position_to_bitboard(2, 0)) == 0 &&
                    (board_state->occupied & position_to_bitboard(1, 0)) == 0 &&
                    (board_state->black_attack & position_to_bitboard(2, 0)) == 0 &&
                    (board_state->black_attack & position_to_bitboard(3, 0)) == 0)

                {
                    BoardState *new_board_state = &stack->boards[stack->count];
                    copy_board(&board_state->board, &new_board_state->board);

                    new_board_state->board.white_pieces.king &= ~position_to_bitboard(4, 0);
                    new_board_state->board.white_pieces.king |= position_to_bitboard(2, 0);
                    new_board_state->board.white_pieces.rooks &= ~position_to_bitboard(0, 0);
                    new_board_state->board.white_pieces.rooks |= position_to_bitboard(3, 0);
                    new_board_state->board.side_to_move = BLACK;
                    new_board_state->board.en_passant = 0;
                    new_board_state->board.castling_rights &= ~WHITE_KINGSIDE_CASTLE;
                    new_board_state->board.castling_rights &= ~WHITE_QUEENSIDE_CASTLE;
                    init_board(new_board_state);
                    new_board_state->has_castled = board_state->has_castled | WHITE_QUEENSIDE_CASTLE;
                    new_board_state->move = new_castling_encoded_move(position_to_index(x, y), position_to_index(2, 0));
                    validate_white_move(stack);
                }
            }
        }
    }
}