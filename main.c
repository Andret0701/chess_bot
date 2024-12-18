#include <stdio.h>
#include "board.h"
#include "engine/board_stack.h"
#include "engine/piece_moves.h"

int main()
{
    BoardStack *stack = create_board_stack(1000);
    Board board;

    stack->count = 0;
    board = (Board){0};
    board.white_pieces.bishops = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.black_pieces.bishops = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.white_pieces.knights = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.black_pieces.knights = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.white_pieces.pawns = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.black_pieces.pawns = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.white_pieces.rooks = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.black_pieces.rooks = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.white_pieces.queens = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.black_pieces.queens = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.white_pieces.king = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    stack->count = 0;
    board = (Board){0};
    board.black_pieces.king = 1;
    generate_moves(&board, stack);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        printf("i: %d\n", i);
        print_board(&stack->boards[i].board);
    }

    destroy_board_stack(stack);
    return 0;
}