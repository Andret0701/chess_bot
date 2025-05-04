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
        new_board_state->board.en_passant = 0;
        new_board_state->board.side_to_move = BLACK;
        init_board(new_board_state);
        new_board_state->has_castled = board_state->has_castled;
        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), false);
        validate_white_move(stack);

        // Two steps forward
        if (y == 1)
        {
            if ((board_state->occupied & position_to_bitboard(x, y + 2)) == 0)
            {
                new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.pawns |= position_to_bitboard(x, y + 2);
                new_board_state->board.en_passant = position_to_bitboard(x, y + 1);
                new_board_state->board.side_to_move = BLACK;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, y + 2), false);
                validate_white_move(stack);
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

            init_board(new_board_state);
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), true);
            validate_white_move(stack);
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
            init_board(new_board_state);
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), true);
            validate_white_move(stack);
        }
    }
}

void white_pawn_promote(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (board_state->occupied & position_to_bitboard(x, y + 1))
        return;

    // Promote to knight
    BoardState *new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
    new_board_state->board.white_pieces.knights |= position_to_bitboard(x, y + 1);
    new_board_state->board.side_to_move = BLACK;
    new_board_state->board.en_passant = 0;
    init_board(new_board_state);
    new_board_state->has_castled = board_state->has_castled;
    new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_KNIGHT, false);
    validate_white_move(stack);

    // Promote to bishop
    new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
    new_board_state->board.white_pieces.bishops |= position_to_bitboard(x, y + 1);
    new_board_state->board.side_to_move = BLACK;
    new_board_state->board.en_passant = 0;
    init_board(new_board_state);
    new_board_state->has_castled = board_state->has_castled;
    new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_BISHOP, false);
    validate_white_move(stack);

    // Promote to rook
    new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
    new_board_state->board.white_pieces.rooks |= position_to_bitboard(x, y + 1);
    new_board_state->board.side_to_move = BLACK;
    new_board_state->board.en_passant = 0;
    init_board(new_board_state);
    new_board_state->has_castled = board_state->has_castled;
    new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_ROOK, false);
    validate_white_move(stack);

    // Promote to queen
    new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
    new_board_state->board.white_pieces.queens |= position_to_bitboard(x, y + 1);
    new_board_state->board.side_to_move = BLACK;
    new_board_state->board.en_passant = 0;
    init_board(new_board_state);
    new_board_state->has_castled = board_state->has_castled;
    new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_QUEEN, false);
    validate_white_move(stack);
}

void white_pawn_promotion_attack(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (x > 0)
    {
        if ((board_state->black_pieces & position_to_bitboard(x - 1, y + 1)) != 0)
        {
            // Promote to knight
            BoardState *new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.knights |= position_to_bitboard(x - 1, y + 1);
            remove_black_piece(new_board_state, x - 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            init_board(new_board_state);
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_KNIGHT, true);
            validate_white_move(stack);

            // Promote to bishop
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.bishops |= position_to_bitboard(x - 1, y + 1);
            remove_black_piece(new_board_state, x - 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            init_board(new_board_state);
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_BISHOP, true);
            validate_white_move(stack);

            // Promote to rook
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.rooks |= position_to_bitboard(x - 1, y + 1);
            remove_black_piece(new_board_state, x - 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            init_board(new_board_state);
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_ROOK, true);
            validate_white_move(stack);

            // Promote to queen
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.queens |= position_to_bitboard(x - 1, y + 1);
            remove_black_piece(new_board_state, x - 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            init_board(new_board_state);
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_QUEEN, true);
            validate_white_move(stack);
        }
    }

    if (x < 7)
    {
        if ((board_state->black_pieces & position_to_bitboard(x + 1, y + 1)) != 0)
        {
            // Promote to knight
            BoardState *new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.knights |= position_to_bitboard(x + 1, y + 1);
            remove_black_piece(new_board_state, x + 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            init_board(new_board_state);
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_KNIGHT, true);
            validate_white_move(stack);

            // Promote to bishop
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.bishops |= position_to_bitboard(x + 1, y + 1);
            remove_black_piece(new_board_state, x + 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            init_board(new_board_state);
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_BISHOP, true);
            validate_white_move(stack);

            // Promote to rook
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.rooks |= position_to_bitboard(x + 1, y + 1);
            remove_black_piece(new_board_state, x + 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            init_board(new_board_state);
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_ROOK, true);
            validate_white_move(stack);

            // Promote to queen
            new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.white_pieces.pawns &= ~position_to_bitboard(x, y);
            new_board_state->board.white_pieces.queens |= position_to_bitboard(x + 1, y + 1);
            remove_black_piece(new_board_state, x + 1, y + 1);

            new_board_state->board.side_to_move = BLACK;
            new_board_state->board.en_passant = 0;
            init_board(new_board_state);
            new_board_state->has_castled = board_state->has_castled;
            new_board_state->move = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_QUEEN, true);
            validate_white_move(stack);
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
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_en_passant_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1));
                validate_white_move(stack);
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
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_en_passant_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1));
                validate_white_move(stack);
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