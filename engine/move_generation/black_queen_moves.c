#include "../piece_moves.h"

void generate_black_queen_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    // Right
    for (uint8_t i = x + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(i, y))
        {
            if (board_state->white_pieces & position_to_bitboard(i, y))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_white_piece(new_board_state, i, y);
                new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.black_pieces.queens |= position_to_bitboard(i, y);
                new_board_state->board.side_to_move = WHITE;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(i, y), true);
                validate_black_move(stack);
            }
            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state->board.black_pieces.queens |= position_to_bitboard(i, y);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        new_board_state->has_castled = board_state->has_castled;
        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(i, y), false);
        validate_black_move(stack);
    }

    // Left
    for (int8_t i = x - 1; i >= 0; i--)
    {
        if (board_state->occupied & position_to_bitboard(i, y))
        {
            if (board_state->white_pieces & position_to_bitboard(i, y))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_white_piece(new_board_state, i, y);
                new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.black_pieces.queens |= position_to_bitboard(i, y);
                new_board_state->board.side_to_move = WHITE;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(i, y), true);
                validate_black_move(stack);
            }
            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state->board.black_pieces.queens |= position_to_bitboard(i, y);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        new_board_state->has_castled = board_state->has_castled;
        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(i, y), false);
        validate_black_move(stack);
    }

    // Up
    for (uint8_t i = y + 1; i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(x, i))
        {
            if (board_state->white_pieces & position_to_bitboard(x, i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_white_piece(new_board_state, x, i);
                new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.black_pieces.queens |= position_to_bitboard(x, i);
                new_board_state->board.side_to_move = WHITE;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, i), true);
                validate_black_move(stack);
            }
            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state->board.black_pieces.queens |= position_to_bitboard(x, i);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        new_board_state->has_castled = board_state->has_castled;
        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, i), false);
        validate_black_move(stack);
    }

    // Down
    for (int8_t i = y - 1; i >= 0; i--)
    {
        if (board_state->occupied & position_to_bitboard(x, i))
        {
            if (board_state->white_pieces & position_to_bitboard(x, i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_white_piece(new_board_state, x, i);
                new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.black_pieces.queens |= position_to_bitboard(x, i);
                new_board_state->board.side_to_move = WHITE;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, i), true);
                validate_black_move(stack);
            }
            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state->board.black_pieces.queens |= position_to_bitboard(x, i);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        new_board_state->has_castled = board_state->has_castled;
        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, i), false);
        validate_black_move(stack);
    }

    // Diagonal movements
    // Up-Right
    for (uint8_t i = 1; x + i < 8 && y + i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(x + i, y + i))
        {
            if (board_state->white_pieces & position_to_bitboard(x + i, y + i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_white_piece(new_board_state, x + i, y + i);
                new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.black_pieces.queens |= position_to_bitboard(x + i, y + i);
                new_board_state->board.side_to_move = WHITE;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x + i, y + i), true);
                validate_black_move(stack);
            }
            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state->board.black_pieces.queens |= position_to_bitboard(x + i, y + i);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        new_board_state->has_castled = board_state->has_castled;
        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x + i, y + i), false);
        validate_black_move(stack);
    }

    // Up-Left
    for (uint8_t i = 1; x - i >= 0 && y + i < 8; i++)
    {
        if (board_state->occupied & position_to_bitboard(x - i, y + i))
        {
            if (board_state->white_pieces & position_to_bitboard(x - i, y + i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_white_piece(new_board_state, x - i, y + i);
                new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.black_pieces.queens |= position_to_bitboard(x - i, y + i);
                new_board_state->board.side_to_move = WHITE;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x - i, y + i), true);
                validate_black_move(stack);
            }
            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state->board.black_pieces.queens |= position_to_bitboard(x - i, y + i);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        new_board_state->has_castled = board_state->has_castled;
        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x - i, y + i), false);
        validate_black_move(stack);
    }

    // Down-Right
    for (uint8_t i = 1; x + i < 8 && y - i >= 0; i++)
    {
        if (board_state->occupied & position_to_bitboard(x + i, y - i))
        {
            if (board_state->white_pieces & position_to_bitboard(x + i, y - i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_white_piece(new_board_state, x + i, y - i);
                new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.black_pieces.queens |= position_to_bitboard(x + i, y - i);
                new_board_state->board.side_to_move = WHITE;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x + i, y - i), true);
                validate_black_move(stack);
            }
            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state->board.black_pieces.queens |= position_to_bitboard(x + i, y - i);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        new_board_state->has_castled = board_state->has_castled;
        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x + i, y - i), false);
        validate_black_move(stack);
    }

    // Down-Left
    for (uint8_t i = 1; x - i >= 0 && y - i >= 0; i++)
    {
        if (board_state->occupied & position_to_bitboard(x - i, y - i))
        {
            if (board_state->white_pieces & position_to_bitboard(x - i, y - i))
            {
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_white_piece(new_board_state, x - i, y - i);
                new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
                new_board_state->board.black_pieces.queens |= position_to_bitboard(x - i, y - i);
                new_board_state->board.side_to_move = WHITE;
                new_board_state->board.en_passant = 0;
                init_board(new_board_state);
                new_board_state->has_castled = board_state->has_castled;
                new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x - i, y - i), true);
                validate_black_move(stack);
            }
            break;
        }

        BoardState *new_board_state = &stack->boards[stack->count];
        copy_board(&board_state->board, &new_board_state->board);
        new_board_state->board.black_pieces.queens &= ~position_to_bitboard(x, y);
        new_board_state->board.black_pieces.queens |= position_to_bitboard(x - i, y - i);
        new_board_state->board.side_to_move = WHITE;
        new_board_state->board.en_passant = 0;
        init_board(new_board_state);
        new_board_state->has_castled = board_state->has_castled;
        new_board_state->move = new_simple_encoded_move(position_to_index(x, y), position_to_index(x - i, y - i), false);
        validate_black_move(stack);
    }
}