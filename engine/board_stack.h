#pragma once
#include <stdint.h>
#include "board_state.h"

#define BOARD_STACK_SIZE 999999

typedef struct
{
    BoardState *boards;
    uint32_t count;
    uint32_t size; // Maximum size of the stack
} BoardStack;

BoardStack *create_board_stack(uint32_t size);
void destroy_board_stack(BoardStack *stack);
