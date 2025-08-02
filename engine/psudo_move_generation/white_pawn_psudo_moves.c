#include "../piece_moves.h"
#include "../move_stack.h"

void white_pawn_advance_psudo(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    // One step forward
    if (!is_piece(board_state, x, y + 1))
    {
        // Move the pawn one step forward
        stack->moves[stack->count++] = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), false);

        // Move the pawn two steps forward if it's on the second rank
        if (y == 1 && !is_piece(board_state, x, y + 2))
            stack->moves[stack->count++] = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, y + 2), false);
    }
}

void white_pawn_attack_psudo(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    // Attack left
    if (x > 0 && is_black_piece(board_state, x - 1, y + 1))
        stack->moves[stack->count++] = new_simple_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), true);

    // Attack right
    if (x < 7 && is_black_piece(board_state, x + 1, y + 1))
        stack->moves[stack->count++] = new_simple_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), true);
}

void white_pawn_promote_psudo(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    if (is_piece(board_state, x, y + 1))
        return;

    // Promote knight
    stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_KNIGHT, false);

    // Promote bishop
    stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_BISHOP, false);

    // Promote new_board_state rook
    stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_ROOK, false);

    // Promote queen
    stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y + 1), PROMOTION_QUEEN, false);
}

void white_pawn_promotion_attack_psudo(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    if (x > 0 && is_black_piece(board_state, x - 1, y + 1))
    {
        // Promote knight
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_KNIGHT, true);

        // Promote bishop
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_BISHOP, true);

        // Promote rook
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_ROOK, true);

        // Promote queen
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1), PROMOTION_QUEEN, true);
    }

    if (x < 7 && is_black_piece(board_state, x + 1, y + 1))
    {
        // Promote knight
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_KNIGHT, true);

        // Promote bishop
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_BISHOP, true);

        // Promote rook
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_ROOK, true);

        // Promote queen
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1), PROMOTION_QUEEN, true);
    }
}

void white_pawn_en_passant_psudo(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    if (y == 4)
    {
        if (x > 0 && is_en_passant(&board_state->board, x - 1, y + 1))
            stack->moves[stack->count++] = new_en_passant_encoded_move(position_to_index(x, y), position_to_index(x - 1, y + 1));

        if (x < 7 && is_en_passant(&board_state->board, x + 1, y + 1))
            stack->moves[stack->count++] = new_en_passant_encoded_move(position_to_index(x, y), position_to_index(x + 1, y + 1));
    }
}

void generate_white_pawn_psudo_moves(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    if (y == 6)
    {
        white_pawn_promote_psudo(board_state, x, y, stack);
        white_pawn_promotion_attack_psudo(board_state, x, y, stack);
        return;
    }

    white_pawn_advance_psudo(board_state, x, y, stack);
    white_pawn_attack_psudo(board_state, x, y, stack);
    white_pawn_en_passant_psudo(board_state, x, y, stack);
}