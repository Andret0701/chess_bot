#include "../piece_moves.h"

void generate_white_bishop_captures(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    // Loop to check moves in the NE direction
    for (uint8_t i = 1; x + i < 8 && y + i < 8; i++)
    {
        uint64_t target_pos = position_to_bitboard(x + i, y + i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                // Capture move
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x + i, y + i);
                new_board_state->board.white_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.bishops |= target_pos;
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                new_board_state->has_castled = board_state->has_castled;
                push_white_move(board_state, new_board_state, stack);
            }

            break;
        }
    }

    // Loop to check moves in the SE direction
    for (uint8_t i = 1; x + i < 8 && y - i >= 0; i++)
    {
        uint64_t target_pos = position_to_bitboard(x + i, y - i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                // Capture move
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x + i, y - i);
                new_board_state->board.white_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.bishops |= target_pos;
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                new_board_state->has_castled = board_state->has_castled;
                push_white_move(board_state, new_board_state, stack);
            }

            break;
        }
    }

    // Loop to check moves in the SW direction
    for (uint8_t i = 1; x - i >= 0 && y + i < 8; i++)
    {
        uint64_t target_pos = position_to_bitboard(x - i, y + i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                // Capture move
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x - i, y + i);
                new_board_state->board.white_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.bishops |= target_pos;
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                new_board_state->has_castled = board_state->has_castled;
                push_white_move(board_state, new_board_state, stack);
            }

            break;
        }
    }

    // Loop to check moves in the NW direction
    for (uint8_t i = 1; x - i >= 0 && y - i >= 0; i++)
    {
        uint64_t target_pos = position_to_bitboard(x - i, y - i);
        if (board_state->occupied & target_pos)
        {
            if (board_state->black_pieces & target_pos)
            {
                // Capture move
                BoardState *new_board_state = &stack->boards[stack->count];
                copy_board(&board_state->board, &new_board_state->board);
                remove_black_piece(new_board_state, x - i, y - i);
                new_board_state->board.white_pieces.bishops &= ~position_to_bitboard(x, y);
                new_board_state->board.white_pieces.bishops |= target_pos;
                new_board_state->board.side_to_move = BLACK;
                new_board_state->board.en_passant = 0;
                new_board_state->has_castled = board_state->has_castled;
                push_white_move(board_state, new_board_state, stack);
            }

            break;
        }
    }
}
