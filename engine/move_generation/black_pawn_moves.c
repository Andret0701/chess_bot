#include "../piece_moves.h"

void black_pawn_advance(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    // One step forward
    if ((board_state->occupied & position_to_u64(x, y - 1)) == 0)
    {
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.pawns |= position_to_u64(x, y - 1);
        new_board_state->board.en_passant = 0;
        new_board_state->board.side_to_move = WHITE;
        init_board(new_board_state);
        validate_black_move(stack);

        // Two steps forward
        if (y == 6)
        {
            if ((board_state->occupied & position_to_u64(x, y - 2)) == 0)
            {
                new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
                new_board_state->board.black_pieces.pawns |= position_to_u64(x, y - 2);
                new_board_state->board.en_passant = position_to_u64(x, y - 1);
                new_board_state->board.side_to_move = WHITE;
                init_board(new_board_state);
                validate_black_move(stack);
            }
        }
    }
}

void black_pawn_attack(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (x > 0)
    {
        if ((board_state->white_pieces & position_to_u64(x - 1, y - 1)) != 0)
        {
            BoardState *new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
            new_board_state->board.black_pieces.pawns |= position_to_u64(x - 1, y - 1);
            remove_white_piece(new_board_state, x - 1, y - 1);

            new_board_state->board.en_passant = 0;
            new_board_state->board.side_to_move = WHITE;

            init_board(new_board_state);
            validate_black_move(stack);
        }
    }

    if (x < 7)
    {
        if ((board_state->white_pieces & position_to_u64(x + 1, y - 1)) != 0)
        {
            BoardState *new_board_state = &stack->boards[stack->count];
            copy_board(&board_state->board, &new_board_state->board);
            new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
            new_board_state->board.black_pieces.pawns |= position_to_u64(x + 1, y - 1);
            remove_white_piece(new_board_state, x + 1, y - 1);

            new_board_state->board.en_passant = 0;
            new_board_state->board.side_to_move = WHITE;

            init_board(new_board_state);
            validate_black_move(stack);
        }
    }
}

void black_pawn_promote(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (board_state->occupied & position_to_u64(x, y - 1))
        return;

    // Promote to knight
    BoardState *new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
    new_board_state->board.black_pieces.knights |= position_to_u64(x, y - 1);
    new_board_state->board.side_to_move = WHITE;
    new_board_state->board.en_passant = 0;
    init_board(new_board_state);
    validate_black_move(stack);

    // Promote to bishop
    new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
    new_board_state->board.black_pieces.bishops |= position_to_u64(x, y - 1);
    new_board_state->board.side_to_move = WHITE;
    new_board_state->board.en_passant = 0;
    init_board(new_board_state);
    validate_black_move(stack);

    // Promote to rook
    new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
    new_board_state->board.black_pieces.rooks |= position_to_u64(x, y - 1);
    new_board_state->board.side_to_move = WHITE;
    new_board_state->board.en_passant = 0;
    init_board(new_board_state);
    validate_black_move(stack);

    // Promote to queen
    new_board_state = &stack->boards[stack->count];
    copy_board(&board_state->board, &new_board_state->board);
    new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
    new_board_state->board.black_pieces.queens |= position_to_u64(x, y - 1);
    new_board_state->board.side_to_move = WHITE;
    new_board_state->board.en_passant = 0;
    init_board(new_board_state);
    validate_black_move(stack);
}

void black_pawn_promotion_attack(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (x > 0 && (board_state->white_pieces & position_to_u64(x - 1, y - 1)) != 0)
    {
        // Promote to knight
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.knights |= position_to_u64(x - 1, y - 1);
        remove_white_piece(new_board_state, x - 1, y - 1);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        validate_black_move(stack);

        // Promote to bishop
        new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.bishops |= position_to_u64(x - 1, y - 1);
        remove_white_piece(new_board_state, x - 1, y - 1);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        validate_black_move(stack);

        // Promote to rook
        new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.rooks |= position_to_u64(x - 1, y - 1);
        remove_white_piece(new_board_state, x - 1, y - 1);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        validate_black_move(stack);

        // Promote to queen
        new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.queens |= position_to_u64(x - 1, y - 1);
        remove_white_piece(new_board_state, x - 1, y - 1);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        validate_black_move(stack);
    }

    if (x < 7 && (board_state->white_pieces & position_to_u64(x + 1, y - 1)) != 0)
    {
        // Promote to knight
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.knights |= position_to_u64(x + 1, y - 1);
        remove_white_piece(new_board_state, x + 1, y - 1);

        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        validate_black_move(stack);

        // Promote to bishop
        new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.bishops |= position_to_u64(x + 1, y - 1);
        remove_white_piece(new_board_state, x + 1, y - 1);

        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        validate_black_move(stack);

        // Promote to rook
        new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.rooks |= position_to_u64(x + 1, y - 1);
        remove_white_piece(new_board_state, x + 1, y - 1);

        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        validate_black_move(stack);

        // Promote to queen
        new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.queens |= position_to_u64(x + 1, y - 1);
        remove_white_piece(new_board_state, x + 1, y - 1);

        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        validate_black_move(stack);
    }
}

void black_pawn_en_passant(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (y == 3)
    {
        if (x > 0)
        {
            if ((board_state->board.en_passant & position_to_u64(x - 1, y - 1)) != 0)
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
                new_board_state->board.black_pieces.pawns |= position_to_u64(x - 1, y - 1);
                new_board_state->board.white_pieces.pawns &= ~position_to_u64(x - 1, y);
                new_board_state->board.en_passant = 0;
                new_board_state->board.side_to_move = WHITE;
                init_board(new_board_state);
                validate_black_move(stack);
            }
        }

        if (x < 7)
        {
            if ((board_state->board.en_passant & position_to_u64(x + 1, y - 1)) != 0)
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                new_board_state->board.black_pieces.pawns &= ~position_to_u64(x, y);
                new_board_state->board.black_pieces.pawns |= position_to_u64(x + 1, y - 1);
                new_board_state->board.white_pieces.pawns &= ~position_to_u64(x + 1, y);
                new_board_state->board.en_passant = 0;
                new_board_state->board.side_to_move = WHITE;
                init_board(new_board_state);
                validate_black_move(stack);
            }
        }
    }
}

void generate_black_pawn_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (y == 1)
    {
        black_pawn_promote(board_state, x, y, stack);
        black_pawn_promotion_attack(board_state, x, y, stack);
        return;
    }

    black_pawn_advance(board_state, x, y, stack);
    black_pawn_attack(board_state, x, y, stack);
    black_pawn_en_passant(board_state, x, y, stack);
}
