#include <stdio.h>
#include "board.h"
#include "engine/board_stack.h"
#include "engine/piece_moves.h"
#include "fen.h"

int main()
{
    BoardStack *stack = create_board_stack(1000);
    Board board = fen_to_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    stack->count = 0;
    generate_moves(&board, stack);

    printf("Number of moves: %d\n", stack->count);

    destroy_board_stack(stack);
    return 0;
}