#include "../piece_moves.h"

void generate_black_king_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    uint64_t king_moves = board_state->black_attacks.king;
    king_moves &= ~board_state->black_pieces;
    king_moves &= ~board_state->white_attack;
    while (king_moves)
    {
        uint8_t sq = __builtin_ctzll(king_moves);
        king_moves &= king_moves - 1;
        uint8_t new_x = sq % 8;
        uint8_t new_y = sq / 8;

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        remove_white_piece(new_board_state, new_x, new_y);

        new_board_state->board.black_pieces.king &= ~position_to_bitboard(x, y);
        new_board_state->board.black_pieces.king |= position_to_bitboard(new_x, new_y);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        new_board_state->board.castling_rights &= ~BLACK_KINGSIDE_CASTLE;
        new_board_state->board.castling_rights &= ~BLACK_QUEENSIDE_CASTLE;
        init_board(new_board_state);
        new_board_state->has_castled = board_state->has_castled;
        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(new_x, new_y), is_white_piece(board_state, new_x, new_y));
        validate_black_move(stack);
    }

    // Castling
    if (x == 4 && y == 7)
    {
        if (!board_state->black_check)
        {
            if ((board_state->board.castling_rights & BLACK_KINGSIDE_CASTLE) != 0)
            {
                if ((board_state->occupied & position_to_bitboard(5, 7)) == 0 &&
                    (board_state->occupied & position_to_bitboard(6, 7)) == 0 &&
                    (board_state->white_attack & position_to_bitboard(5, 7)) == 0)
                {
                    BoardState *new_board_state = &stack->boards[stack->count];
                    copy_board(&board_state->board, &new_board_state->board);

                    new_board_state->board.black_pieces.king &= ~position_to_bitboard(4, 7);
                    new_board_state->board.black_pieces.king |= position_to_bitboard(6, 7);
                    new_board_state->board.black_pieces.rooks &= ~position_to_bitboard(7, 7);
                    new_board_state->board.black_pieces.rooks |= position_to_bitboard(5, 7);
                    new_board_state->board.side_to_move = WHITE;
                    new_board_state->board.en_passant = 0;
                    new_board_state->board.castling_rights &= ~BLACK_KINGSIDE_CASTLE;
                    new_board_state->board.castling_rights &= ~BLACK_QUEENSIDE_CASTLE;
                    init_board(new_board_state);
                    new_board_state->has_castled = board_state->has_castled | BLACK_KINGSIDE_CASTLE;
                    new_board_state->move = new_castling_encoded_move(position_to_index(x, y), position_to_index(6, 7));
                    validate_black_move(stack);
                }
            }

            if ((board_state->board.castling_rights & BLACK_QUEENSIDE_CASTLE) != 0)
            {
                if ((board_state->occupied & position_to_bitboard(1, 7)) == 0 &&
                    (board_state->occupied & position_to_bitboard(2, 7)) == 0 &&
                    (board_state->occupied & position_to_bitboard(3, 7)) == 0 &&
                    (board_state->white_attack & position_to_bitboard(2, 7)) == 0 &&
                    (board_state->white_attack & position_to_bitboard(3, 7)) == 0)
                {
                    BoardState *new_board_state = &stack->boards[stack->count];
                    copy_board(&board_state->board, &new_board_state->board);

                    new_board_state->board.black_pieces.king &= ~position_to_bitboard(4, 7);
                    new_board_state->board.black_pieces.king |= position_to_bitboard(2, 7);
                    new_board_state->board.black_pieces.rooks &= ~position_to_bitboard(0, 7);
                    new_board_state->board.black_pieces.rooks |= position_to_bitboard(3, 7);
                    new_board_state->board.side_to_move = WHITE;
                    new_board_state->board.en_passant = 0;
                    new_board_state->board.castling_rights &= ~BLACK_KINGSIDE_CASTLE;
                    new_board_state->board.castling_rights &= ~BLACK_QUEENSIDE_CASTLE;
                    init_board(new_board_state);
                    new_board_state->has_castled = board_state->has_castled | BLACK_QUEENSIDE_CASTLE;
                    new_board_state->move = new_castling_encoded_move(position_to_index(x, y), position_to_index(2, 7));
                    validate_black_move(stack);
                }
            }
        }
    }
}
