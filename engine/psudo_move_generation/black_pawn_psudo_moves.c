#include "../piece_moves.h"
#include "../move_stack.h"

void black_pawn_advance_psudo(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    // One step forward (downward for black)
    if (!is_piece(board_state, x, y - 1))
    {
        // Move the pawn one step forward
        stack->moves[stack->count++] = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, y - 1), false);

        // Move the pawn two steps forward if it's on the seventh rank
        if (y == 6 && !is_piece(board_state, x, y - 2))
            stack->moves[stack->count++] = new_simple_encoded_move(position_to_index(x, y), position_to_index(x, y - 2), false);
    }
}

void black_pawn_attack_psudo(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    // Attack left (down-left)
    if (x > 0 && is_white_piece(board_state, x - 1, y - 1))
        stack->moves[stack->count++] = new_simple_encoded_move(position_to_index(x, y), position_to_index(x - 1, y - 1), true);

    // Attack right (down-right)
    if (x < 7 && is_white_piece(board_state, x + 1, y - 1))
        stack->moves[stack->count++] = new_simple_encoded_move(position_to_index(x, y), position_to_index(x + 1, y - 1), true);
}

void black_pawn_promote_psudo(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    if (is_piece(board_state, x, y - 1))
        return;

    // Promote knight
    stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y - 1), PROMOTION_KNIGHT, false);

    // Promote bishop
    stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y - 1), PROMOTION_BISHOP, false);

    // Promote rook
    stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y - 1), PROMOTION_ROOK, false);

    // Promote queen
    stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x, y - 1), PROMOTION_QUEEN, false);
}

void black_pawn_promotion_attack_psudo(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    if (x > 0 && is_white_piece(board_state, x - 1, y - 1))
    {
        // Promote knight
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y - 1), PROMOTION_KNIGHT, true);

        // Promote bishop
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y - 1), PROMOTION_BISHOP, true);

        // Promote rook
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y - 1), PROMOTION_ROOK, true);

        // Promote queen
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x - 1, y - 1), PROMOTION_QUEEN, true);
    }

    if (x < 7 && is_white_piece(board_state, x + 1, y - 1))
    {
        // Promote knight
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y - 1), PROMOTION_KNIGHT, true);

        // Promote bishop
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y - 1), PROMOTION_BISHOP, true);

        // Promote rook
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y - 1), PROMOTION_ROOK, true);

        // Promote queen
        stack->moves[stack->count++] = new_promotion_encoded_move(position_to_index(x, y), position_to_index(x + 1, y - 1), PROMOTION_QUEEN, true);
    }
}

void black_pawn_en_passant_psudo(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    if (y == 3)
    {
        if (x > 0 && is_en_passant(&board_state->board, x - 1, y - 1))
            stack->moves[stack->count++] = new_en_passant_encoded_move(position_to_index(x, y), position_to_index(x - 1, y - 1));

        if (x < 7 && is_en_passant(&board_state->board, x + 1, y - 1))
            stack->moves[stack->count++] = new_en_passant_encoded_move(position_to_index(x, y), position_to_index(x + 1, y - 1));
    }
}

void generate_black_pawn_psudo_moves(BoardState *board_state, uint8_t x, uint8_t y, MoveStack *stack)
{
    if (y == 1)
    {
        black_pawn_promote_psudo(board_state, x, y, stack);
        black_pawn_promotion_attack_psudo(board_state, x, y, stack);
        return;
    }

    black_pawn_advance_psudo(board_state, x, y, stack);
    black_pawn_attack_psudo(board_state, x, y, stack);
    black_pawn_en_passant_psudo(board_state, x, y, stack);
}