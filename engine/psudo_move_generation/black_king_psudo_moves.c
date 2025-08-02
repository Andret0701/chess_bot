#include "../piece_moves.h"

void generate_black_king_psudo_moves(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
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
            if (!is_black_piece(board_state, new_x, new_y))
            {
                stack->moves[stack->count++] = new_simple_encoded_move(
                    position_to_index(x, y),
                    position_to_index(new_x, new_y),
                    is_white_piece(board_state, new_x, new_y));
            }
        }
    }

    if (!board_state->black_check)
    {
        // Castling - Kingside
        if ((board_state->board.castling_rights & BLACK_KINGSIDE_CASTLE) &&
            !is_piece(board_state, 5, 7) &&
            !is_piece(board_state, 6, 7) &&
            (board_state->white_attack & position_to_bitboard(5, 7)) == 0)
        {
            stack->moves[stack->count++] = new_castling_encoded_move(
                position_to_index(4, 7),
                position_to_index(6, 7));
        }

        // Castling - Queenside
        if ((board_state->board.castling_rights & BLACK_QUEENSIDE_CASTLE) &&
            !is_piece(board_state, 3, 7) &&
            !is_piece(board_state, 2, 7) &&
            !is_piece(board_state, 1, 7) &&
            (board_state->white_attack & position_to_bitboard(2, 7)) == 0 &&
            (board_state->white_attack & position_to_bitboard(3, 7)) == 0)
        {
            stack->moves[stack->count++] = new_castling_encoded_move(
                position_to_index(4, 7),
                position_to_index(2, 7));
        }
    }
}