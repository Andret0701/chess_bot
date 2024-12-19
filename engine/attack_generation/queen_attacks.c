#include "attack_generation.h"

uint64_t generate_queen_attacks(BoardState *board_state, uint8_t x, uint8_t y)
{
    return generate_bishop_attacks(board_state, x, y) | generate_rook_attacks(board_state, x, y);
}