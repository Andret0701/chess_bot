#include "piece_moves.h"

static inline void generate_white_rook_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    for (uint8_t i = x + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_u64(i, y))
        {
            if (board_state->black_pieces & position_to_u64(i, y))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, i, y);
                new_board_state->board.white_pieces.rooks &= ~position_to_u64(x, y);
                new_board_state->board.white_pieces.rooks |= position_to_u64(i, y);
                new_board_state->board.side_to_move = BLACK;
                init_board(new_board_state);
                validate_white_move(stack);
            }

            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.white_pieces.rooks &= ~position_to_u64(x, y);
        new_board_state->board.white_pieces.rooks |= position_to_u64(i, y);
        new_board_state->board.side_to_move = BLACK;
        init_board(new_board_state);
        validate_white_move(stack);
    }

    for (uint8_t i = x - 1; i < 8; i--)
    {
        if (board_state->occupied & position_to_u64(i, y))
        {
            if (board_state->black_pieces & position_to_u64(i, y))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, i, y);
                new_board_state->board.white_pieces.rooks &= ~position_to_u64(x, y);
                new_board_state->board.white_pieces.rooks |= position_to_u64(i, y);
                new_board_state->board.side_to_move = BLACK;
                init_board(new_board_state);
                validate_white_move(stack);
            }

            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.white_pieces.rooks &= ~position_to_u64(x, y);
        new_board_state->board.white_pieces.rooks |= position_to_u64(i, y);
        new_board_state->board.side_to_move = BLACK;
        init_board(new_board_state);
        validate_white_move(stack);
    }

    for (uint8_t i = y + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_u64(x, i))
        {
            if (board_state->black_pieces & position_to_u64(x, i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x, i);
                new_board_state->board.white_pieces.rooks &= ~position_to_u64(x, y);
                new_board_state->board.white_pieces.rooks |= position_to_u64(x, i);
                new_board_state->board.side_to_move = BLACK;
                init_board(new_board_state);
                validate_white_move(stack);
            }

            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.white_pieces.rooks &= ~position_to_u64(x, y);
        new_board_state->board.white_pieces.rooks |= position_to_u64(x, i);
        new_board_state->board.side_to_move = BLACK;
        init_board(new_board_state);
        validate_white_move(stack);
    }

    for (uint8_t i = y - 1; i < 8; i--)
    {
        if (board_state->occupied & position_to_u64(x, i))
        {
            if (board_state->black_pieces & position_to_u64(x, i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x, i);
                new_board_state->board.white_pieces.rooks &= ~position_to_u64(x, y);
                new_board_state->board.white_pieces.rooks |= position_to_u64(x, i);
                new_board_state->board.side_to_move = BLACK;
                init_board(new_board_state);
                validate_white_move(stack);
            }

            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.white_pieces.rooks &= ~position_to_u64(x, y);
        new_board_state->board.white_pieces.rooks |= position_to_u64(x, i);
        new_board_state->board.side_to_move = BLACK;
        init_board(new_board_state);
        validate_white_move(stack);
    }
}