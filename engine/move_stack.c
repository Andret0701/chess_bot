#include "move_stack.h"
#include <stdlib.h>

MoveStack *create_move_stack(uint32_t size)
{
    MoveStack *stack = malloc(sizeof(MoveStack));
    stack->moves = malloc(size * sizeof(uint16_t));
    stack->count = 0;
    stack->size = size;
    return stack;
}

void destroy_move_stack(MoveStack *stack)
{
    free(stack->moves);
    free(stack);
}