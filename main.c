#include <stdio.h>
#include "board.h"
#include "engine/board_stack.h"
#include "engine/piece_moves.h"

int main()
{
    Board board = {0};
    board.white_pieces.bishops = 1;
    print_board(&board);
    //  board.white_pieces.knights = 1;
    BoardStack *stack = create_board_stack(1000);
    printf("Hello, World!\n");
    generate_moves(&board, stack);
    printf("count: %d\n", stack->count);

    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    board.white_pieces.bishops = 1;
    if (board.white_pieces.bishops == 1)
    {
        printf("Success\n");
    }
    else
    {
        printf("Failure\n");
    }
    printf("Hello, uuWorld!\n");
    return 0;
}