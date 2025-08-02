#include "../piece_moves.h"

void generate_white_king_psudo_moves(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    static const int king_moves[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    // Normal king moves
    for (int i = 0; i < 8; ++i)
    {
        int new_x = x + king_moves[i][0];
        int new_y = y + king_moves[i][1];

        if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
        {
            if (!is_white_piece(board_state, new_x, new_y))
            {
                stack->moves[stack->count++] = new_simple_encoded_move(
                    position_to_index(x, y),
                    position_to_index(new_x, new_y),
                    is_black_piece(board_state, new_x, new_y));
            }
        }
    }

    if (!board_state->white_check)
    {
        // Castling - Kingside
        if ((board_state->board.castling_rights & WHITE_KINGSIDE_CASTLE) &&
            !is_piece(board_state, 5, 0) &&
            !is_piece(board_state, 6, 0) &&
            (board_state->black_attack & position_to_bitboard(5, 0)) == 0)
        {
            stack->moves[stack->count++] = new_castling_encoded_move(
                position_to_index(4, 0),
                position_to_index(6, 0));
        }

        // Castling - Queenside
        if ((board_state->board.castling_rights & WHITE_QUEENSIDE_CASTLE) &&
            !is_piece(board_state, 3, 0) &&
            !is_piece(board_state, 2, 0) &&
            !is_piece(board_state, 1, 0) &&
            (board_state->black_attack & position_to_bitboard(2, 0)) == 0 &&
            (board_state->black_attack & position_to_bitboard(3, 0)) == 0)
        {
            stack->moves[stack->count++] = new_castling_encoded_move(
                position_to_index(4, 0),
                position_to_index(2, 0));
        }
    }
}