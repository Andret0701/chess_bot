#include "../piece_moves.h"

void white_pawn_advance(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    // One step forward
    if ((board_state->occupied & position_to_bitboard(x, y + 1)) == 0)
    {
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
        new_board_state->board.white_pieces.pawns |= position_to_bitboard(x, y + 1);
        // push_white_pawn
        {
            new_board_state->white_pieces = (board_state->white_pieces & ~board_state->board.white_pieces.pawns) | new_board_state->board.white_pieces.pawns;
            new_board_state->black_pieces = board_state->black_pieces;
            new_board_state->occupied = new_board_state->white_pieces | new_board_state->black_pieces;

            // generate_black_attacks
            uint64_t occupied = new_board_state->occupied;
            uint64_t occupied_delta = board_state->occupied ^ new_board_state->occupied;
            {
                new_board_state->black_attacks.pawns = board_state->black_attacks.pawns;
                new_board_state->black_attacks.knights = board_state->black_attacks.knights;
                new_board_state->black_attacks.king = board_state->black_attacks.king;

                if (occupied_delta & board_state->black_attacks.bishops)
                    new_board_state->black_attacks.bishops = generate_bishop_attacks(occupied, new_board_state->board.black_pieces.bishops);
                else
                    new_board_state->black_attacks.bishops = board_state->black_attacks.bishops;
                if (occupied_delta & board_state->black_attacks.rooks)
                    new_board_state->black_attacks.rooks = generate_rook_attacks(occupied, new_board_state->board.black_pieces.rooks);
                else
                    new_board_state->black_attacks.rooks = board_state->black_attacks.rooks;
                if (occupied_delta & board_state->black_attacks.queens)
                    new_board_state->black_attacks.queens = generate_queen_attacks(occupied, new_board_state->board.black_pieces.queens);
                else
                    new_board_state->black_attacks.queens = board_state->black_attacks.queens;

                new_board_state->black_attack = new_board_state->black_attacks.pawns | new_board_state->black_attacks.knights | new_board_state->black_attacks.bishops | new_board_state->black_attacks.rooks | new_board_state->black_attacks.queens | new_board_state->black_attacks.king;
            }
            new_board_state->white_check = new_board_state->black_attack & new_board_state->board.white_pieces.king;
            if (__builtin_expect(!new_board_state->white_check, 1))
            {
                stack->count++;
                // generate_white_attacks
                {
                    new_board_state->white_attacks.pawns = generate_white_pawn_attacks(new_board_state->board.white_pieces.pawns);
                    new_board_state->white_attacks.knights = board_state->white_attacks.knights;
                    new_board_state->white_attacks.king = board_state->white_attacks.king;

                    if (occupied_delta & board_state->white_attacks.bishops)
                        new_board_state->white_attacks.bishops = generate_bishop_attacks(occupied, new_board_state->board.white_pieces.bishops);
                    else
                        new_board_state->white_attacks.bishops = board_state->white_attacks.bishops;
                    if (occupied_delta & board_state->white_attacks.rooks)
                        new_board_state->white_attacks.rooks = generate_rook_attacks(occupied, new_board_state->board.white_pieces.rooks);
                    else
                        new_board_state->white_attacks.rooks = board_state->white_attacks.rooks;
                    if (occupied_delta & board_state->white_attacks.queens)
                        new_board_state->white_attacks.queens = generate_queen_attacks(occupied, new_board_state->board.white_pieces.queens);
                    else
                        new_board_state->white_attacks.queens = board_state->white_attacks.queens;

                    new_board_state->white_attack = new_board_state->white_attacks.pawns | new_board_state->white_attacks.knights | new_board_state->white_attacks.bishops | new_board_state->white_attacks.rooks | new_board_state->white_attacks.queens | new_board_state->white_attacks.king;
                }
                new_board_state->black_check = new_board_state->white_attack & new_board_state->board.black_pieces.king;

                new_board_state->board.en_passant = 0;
                new_board_state->board.side_to_move = BLACK;
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), false);
            }
        }

        // Two steps forward
        if (y == 1)
        {
            if ((board_state->occupied & position_to_bitboard(x, y + 2)) == 0)
            {
                new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.pawns |= position_to_bitboard(x, y + 2);
                // push_white_pawn
                {
                    new_board_state->white_pieces = (board_state->white_pieces & ~board_state->board.white_pieces.pawns) | new_board_state->board.white_pieces.pawns;
                    new_board_state->black_pieces = board_state->black_pieces;
                    new_board_state->occupied = new_board_state->white_pieces | new_board_state->black_pieces;

                    // generate_black_attacks
                    uint64_t occupied = new_board_state->occupied;
                    uint64_t occupied_delta = board_state->occupied ^ new_board_state->occupied;
                    {
                        new_board_state->black_attacks.pawns = board_state->black_attacks.pawns;
                        new_board_state->black_attacks.knights = board_state->black_attacks.knights;
                        new_board_state->black_attacks.king = board_state->black_attacks.king;

                        if (occupied_delta & board_state->black_attacks.bishops)
                            new_board_state->black_attacks.bishops = generate_bishop_attacks(occupied, new_board_state->board.black_pieces.bishops);
                        else
                            new_board_state->black_attacks.bishops = board_state->black_attacks.bishops;
                        if (occupied_delta & board_state->black_attacks.rooks)
                            new_board_state->black_attacks.rooks = generate_rook_attacks(occupied, new_board_state->board.black_pieces.rooks);
                        else
                            new_board_state->black_attacks.rooks = board_state->black_attacks.rooks;
                        if (occupied_delta & board_state->black_attacks.queens)
                            new_board_state->black_attacks.queens = generate_queen_attacks(occupied, new_board_state->board.black_pieces.queens);
                        else
                            new_board_state->black_attacks.queens = board_state->black_attacks.queens;

                        new_board_state->black_attack = new_board_state->black_attacks.pawns | new_board_state->black_attacks.knights | new_board_state->black_attacks.bishops | new_board_state->black_attacks.rooks | new_board_state->black_attacks.queens | new_board_state->black_attacks.king;
                    }
                    new_board_state->white_check = new_board_state->black_attack & new_board_state->board.white_pieces.king;
                    if (__builtin_expect(!new_board_state->white_check, 1))
                    {
                        stack->count++;
                        // generate_white_attacks
                        {
                            new_board_state->white_attacks.pawns = generate_white_pawn_attacks(new_board_state->board.white_pieces.pawns);
                            new_board_state->white_attacks.knights = board_state->white_attacks.knights;
                            new_board_state->white_attacks.king = board_state->white_attacks.king;

                            if (occupied_delta & board_state->white_attacks.bishops)
                                new_board_state->white_attacks.bishops = generate_bishop_attacks(occupied, new_board_state->board.white_pieces.bishops);
                            else
                                new_board_state->white_attacks.bishops = board_state->white_attacks.bishops;
                            if (occupied_delta & board_state->white_attacks.rooks)
                                new_board_state->white_attacks.rooks = generate_rook_attacks(occupied, new_board_state->board.white_pieces.rooks);
                            else
                                new_board_state->white_attacks.rooks = board_state->white_attacks.rooks;
                            if (occupied_delta & board_state->white_attacks.queens)
                                new_board_state->white_attacks.queens = generate_queen_attacks(occupied, new_board_state->board.white_pieces.queens);
                            else
                                new_board_state->white_attacks.queens = board_state->white_attacks.queens;

                            new_board_state->white_attack = new_board_state->white_attacks.pawns | new_board_state->white_attacks.knights | new_board_state->white_attacks.bishops | new_board_state->white_attacks.rooks | new_board_state->white_attacks.queens | new_board_state->white_attacks.king;
                        }
                        new_board_state->black_check = new_board_state->white_attack & new_board_state->board.black_pieces.king;

                        new_board_state->board.en_passant = position_to_bitboard(x, y + 1);
                        new_board_state->board.side_to_move = BLACK;
                        new_board_state->has_castled = board_state->has_castled;
                        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, y + 2), false);
                    }
                }
            }
        }
    }
}

void white_pawn_attack(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (x > 0)
    {
        if ((board_state->black_pieces & position_to_bitboard(x - 1, y + 1)) != 0)
        {
            BoardState *new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.pawns |= position_to_bitboard(x - 1, y + 1);
            remove_black_piece(new_board_state, x - 1, y + 1);

            new_board_state->board.en_passant = 0;
            new_board_state->board.side_to_move = BLACK;

            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), true);
            push_white_move(board_state, new_board_state, stack);
        }
    }

    if (x < 7)
    {
        if ((board_state->black_pieces & position_to_bitboard(x + 1, y + 1)) != 0)
        {
            BoardState *new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.pawns |= position_to_bitboard(x + 1, y + 1);
            remove_black_piece(new_board_state, x + 1, y + 1);

            new_board_state->board.en_passant = 0;
            new_board_state->board.side_to_move = BLACK;
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), true);
            push_white_move(board_state, new_board_state, stack);
        }
    }
}

void white_pawn_promote(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (board_state->occupied & position_to_bitboard(x, y + 1))
        return;

    // Promote new_board_state knight
    BoardState *new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
    new_board_state->board.white_pieces.knights |= position_to_bitboard(x, y + 1);
    new_board_state->board.side_to_move = BLACK;
    new_board_state->board.en_passant = 0;
    new_board_state->has_castled = board_state->has_castled;
    new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_KNIGHT, false);
    push_white_move(board_state, new_board_state, stack);

    // Promote new_board_state bishop
    new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
    new_board_state->board.white_pieces.bishops |= position_to_bitboard(x, y + 1);
    new_board_state->board.side_to_move = BLACK;
    new_board_state->board.en_passant = 0;
    new_board_state->has_castled = board_state->has_castled;
    new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_BISHOP, false);
    push_white_move(board_state, new_board_state, stack);

    // Promote new_board_state rook
    new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
    new_board_state->board.white_pieces.rooks |= position_to_bitboard(x, y + 1);
    new_board_state->board.side_to_move = BLACK;
    new_board_state->board.en_passant = 0;
    new_board_state->has_castled = board_state->has_castled;
    new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_ROOK, false);
    push_white_move(board_state, new_board_state, stack);

    // Promote new_board_state queen
    new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
    new_board_state->board.white_pieces.queens |= position_to_bitboard(x, y + 1);
    new_board_state->board.side_to_move = BLACK;
    new_board_state->board.en_passant = 0;
    new_board_state->has_castled = board_state->has_castled;
    new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_QUEEN, false);
    push_white_move(board_state, new_board_state, stack);
}

void white_pawn_promotion_attack(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (x > 0)
    {
        if ((board_state->black_pieces & position_to_bitboard(x - 1, y + 1)) != 0)
        {
            // Promote new_board_state knight
            BoardState *new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.knights |= position_to_bitboard(x - 1, y + 1);
            remove_black_piece(new_board_state, x - 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_KNIGHT, true);
            push_white_move(board_state, new_board_state, stack);

            // Promote new_board_state bishop
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.bishops |= position_to_bitboard(x - 1, y + 1);
            remove_black_piece(new_board_state, x - 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_BISHOP, true);
            push_white_move(board_state, new_board_state, stack);

            // Promote new_board_state rook
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.rooks |= position_to_bitboard(x - 1, y + 1);
            remove_black_piece(new_board_state, x - 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_ROOK, true);
            push_white_move(board_state, new_board_state, stack);

            // Promote new_board_state queen
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.queens |= position_to_bitboard(x - 1, y + 1);
            remove_black_piece(new_board_state, x - 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_QUEEN, true);
            push_white_move(board_state, new_board_state, stack);
        }
    }

    if (x < 7)
    {
        if ((board_state->black_pieces & position_to_bitboard(x + 1, y + 1)) != 0)
        {
            // Promote new_board_state knight
            BoardState *new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.knights |= position_to_bitboard(x + 1, y + 1);
            remove_black_piece(new_board_state, x + 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_KNIGHT, true);
            push_white_move(board_state, new_board_state, stack);

            // Promote new_board_state bishop
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.bishops |= position_to_bitboard(x + 1, y + 1);
            remove_black_piece(new_board_state, x + 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_BISHOP, true);
            push_white_move(board_state, new_board_state, stack);

            // Promote new_board_state rook
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.rooks |= position_to_bitboard(x + 1, y + 1);
            remove_black_piece(new_board_state, x + 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_ROOK, true);
            push_white_move(board_state, new_board_state, stack);

            // Promote new_board_state queen
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.queens |= position_to_bitboard(x + 1, y + 1);
            remove_black_piece(new_board_state, x + 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_QUEEN, true);
            push_white_move(board_state, new_board_state, stack);
        }
    }
}

void white_pawn_en_passant(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (y == 4)
    {
        if (x > 0)
        {
            if ((board_state->board.en_passant & position_to_bitboard(x - 1, y + 1)) != 0)
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.pawns |= position_to_bitboard(x - 1, y + 1);
                new_board_state->board.black_pieces.pawns &= ~position_to_bitboard(x - 1, y);
                new_board_state->board.en_passant = 0;
                new_board_state->board.side_to_move = BLACK;
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_en_passant_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1));
                push_white_move(board_state, new_board_state, stack);
            }
        }

        if (x < 7)
        {
            if ((board_state->board.en_passant & position_to_bitboard(x + 1, y + 1)) != 0)
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.pawns |= position_to_bitboard(x + 1, y + 1);
                new_board_state->board.black_pieces.pawns &= ~position_to_bitboard(x + 1, y);
                new_board_state->board.en_passant = 0;
                new_board_state->board.side_to_move = BLACK;
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_en_passant_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1));
                push_white_move(board_state, new_board_state, stack);
            }
        }
    }
}

void generate_white_pawn_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (y == 6)
    {
        white_pawn_promote(board_state, x, y, stack);
        white_pawn_promotion_attack(board_state, x, y, stack);
        return;
    }

    white_pawn_advance(board_state, x, y, stack);
    white_pawn_attack(board_state, x, y, stack);
    white_pawn_en_passant(board_state, x, y, stack);
}