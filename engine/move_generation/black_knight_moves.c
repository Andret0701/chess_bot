#include "piece_moves.h"

static inline void generate_black_knight_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    if (board_state->black_pieces & position_to_u64(x + 1, y + 2))
    {
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        remove_white_piece(board_state, x + 1, y + 2);
        new_board_state->board.black_pieces.knights &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.knights |= position_to_u64(x + 1, y + 2);
        new_board_state->board.side_to_move = WHITE;
        init_board(new_board_state);
        validate_white_move(stack);
    }

    if (board_state->black_pieces & position_to_u64(x + 2, y + 1))
    {
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        remove_white_piece(board_state, x + 2, y + 1);
        new_board_state->board.black_pieces.knights &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.knights |= position_to_u64(x + 2, y + 1);
        new_board_state->board.side_to_move = WHITE;
        init_board(new_board_state);
        validate_white_move(stack);
    }

    if (board_state->black_pieces & position_to_u64(x + 2, y - 1))
    {
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        remove_white_piece(board_state, x + 2, y - 1);
        new_board_state->board.black_pieces.knights &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.knights |= position_to_u64(x + 2, y - 1);
        new_board_state->board.side_to_move = WHITE;
        init_board(new_board_state);
        validate_white_move(stack);
    }

    if (board_state->black_pieces & position_to_u64(x + 1, y - 2))
    {
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        remove_white_piece(board_state, x + 1, y - 2);
        new_board_state->board.black_pieces.knights &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.knights |= position_to_u64(x + 1, y - 2);
        new_board_state->board.side_to_move = WHITE;
        init_board(new_board_state);
        validate_white_move(stack);
    }

    if (board_state->black_pieces & position_to_u64(x - 1, y - 2))
    {
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        remove_white_piece(board_state, x - 1, y - 2);
        new_board_state->board.black_pieces.knights &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.knights |= position_to_u64(x - 1, y - 2);
        new_board_state->board.side_to_move = WHITE;
        init_board(new_board_state);
        validate_white_move(stack);
    }

    if (board_state->black_pieces & position_to_u64(x - 2, y - 1))
    {
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        remove_white_piece(board_state, x - 2, y - 1);
        new_board_state->board.black_pieces.knights &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.knights |= position_to_u64(x - 2, y - 1);
        new_board_state->board.side_to_move = WHITE;
        init_board(new_board_state);
        validate_white_move(stack);
    }

    if (board_state->black_pieces & position_to_u64(x - 2, y + 1))
    {
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        remove_white_piece(board_state, x - 2, y + 1);
        new_board_state->board.black_pieces.knights &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.knights |= position_to_u64(x - 2, y + 1);
        new_board_state->board.side_to_move = WHITE;
        init_board(new_board_state);
        validate_white_move(stack);
    }

    if (board_state->black_pieces & position_to_u64(x - 1, y + 2))
    {
        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        remove_white_piece(board_state, x - 1, y + 2);
        new_board_state->board.black_pieces.knights &= ~position_to_u64(x, y);
        new_board_state->board.black_pieces.knights |= position_to_u64(x - 1, y + 2);
        new_board_state->board.side_to_move = WHITE;
        init_board(new_board_state);
        validate_white_move(stack);
    }
}
