#pragma once
#include <stdint.h>
#include "board_state.h"

typedef struct
{
    uint16_t *moves;
    uint32_t count;
    uint32_t size; // Maximum size of the stack
} MoveStack;

MoveStack *create_move_stack(uint32_t size);
void destroy_move_stack(MoveStack *stack);
