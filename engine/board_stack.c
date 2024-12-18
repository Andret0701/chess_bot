#include "board_stack.h"
#include <stdlib.h>

BoardStack *create_board_stack(uint16_t size)
{
    BoardStack *stack = malloc(sizeof(BoardStack));
    stack->boards = malloc(size * sizeof(Board));
    stack->count = 0;
    return stack;
}

void destroy_board_stack(BoardStack *stack)
{
    free(stack->boards);
    free(stack);
}