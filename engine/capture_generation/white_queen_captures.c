#include "../piece_moves.h"

void generate_white_queen_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    // Right
    for (uint8_t i = x + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(i, y))
        {
            if (board_state->black_pieces & position_to_bitboard(i, y))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, i, y);
                new_board_state->board.white_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.queens |= position_to_bitboard(i, y);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                validate_white_move(stack);
            }
            break;
        }
    }

    // Left
    for (int8_t i = x - 1; i >= 0; i--)
    {
        if (board_state->occupied & position_to_bitboard(i, y))
        {
            if (board_state->black_pieces & position_to_bitboard(i, y))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, i, y);
                new_board_state->board.white_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.queens |= position_to_bitboard(i, y);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                validate_white_move(stack);
            }
            break;
        }
    }

    // Up
    for (uint8_t i = y + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(x, i))
        {
            if (board_state->black_pieces & position_to_bitboard(x, i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x, i);
                new_board_state->board.white_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.queens |= position_to_bitboard(x, i);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                validate_white_move(stack);
            }
            break;
        }
    }

    // Down
    for (int8_t i = y - 1; i >= 0; i--)
    {
        if (board_state->occupied & position_to_bitboard(x, i))
        {
            if (board_state->black_pieces & position_to_bitboard(x, i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x, i);
                new_board_state->board.white_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.queens |= position_to_bitboard(x, i);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                validate_white_move(stack);
            }
            break;
        }
    }

    // Diagonal movements
    // Up-Right
    for (uint8_t i = 1; x + i < 8 && y + i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(x + i, y + i))
        {
            if (board_state->black_pieces & position_to_bitboard(x + i, y + i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x + i, y + i);
                new_board_state->board.white_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.queens |= position_to_bitboard(x + i, y + i);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                validate_white_move(stack);
            }
            break;
        }
    }

    // Up-Left
    for (uint8_t i = 1; x - i >= 0 && y + i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(x - i, y + i))
        {
            if (board_state->black_pieces & position_to_bitboard(x - i, y + i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x - i, y + i);
                new_board_state->board.white_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.queens |= position_to_bitboard(x - i, y + i);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                validate_white_move(stack);
            }
            break;
        }
    }

    // Down-Right
    for (uint8_t i = 1; x + i < 8 && y - i >= 0; i++)
    {
        if (board_state->occupied & position_to_bitboard(x + i, y - i))
        {
            if (board_state->black_pieces & position_to_bitboard(x + i, y - i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x + i, y - i);
                new_board_state->board.white_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.queens |= position_to_bitboard(x + i, y - i);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                validate_white_move(stack);
            }
            break;
        }
    }

    // Down-Left
    for (uint8_t i = 1; x - i >= 0 && y - i >= 0; i++)
    {
        if (board_state->occupied & position_to_bitboard(x - i, y - i))
        {
            if (board_state->black_pieces & position_to_bitboard(x - i, y - i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x - i, y - i);
                new_board_state->board.white_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.queens |= position_to_bitboard(x - i, y - i);
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                validate_white_move(stack);
            }
            break;
        }
    }
}