#include "piece_moves.h"

static inline void generate_white_king_moves(BoardState *board_state, uint8_t x, uint8_t y, BoardStack *stack)
{
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;
            if (x + dx >= 0 && x + dx < 8 && y + dy >= 0 && y + dy < 8)
            {
                if (board_state->black_pieces & position_to_u64(x + dx, y + dy))
                {
                    BoardState *new_board_state = &stack->boards[stack->count];
                    copy_board(&board_state->board, &new_board_state->board);
                    remove_black_piece(new_board_state, x + dx, y + dy);
                    new_board_state->board.white_pieces.king &= ~position_to_u64(x, y);
                    new_board_state->board.white_pieces.king |= position_to_u64(x + dx, y + dy);
                    new_board_state->board.side_to_move = BLACK;
                    init_board(new_board_state);
                    validate_white_move(stack);
                }
            }
        }
    }
}