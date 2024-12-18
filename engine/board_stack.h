#pragma once
#include <stdint.h>
#include "board.h"

typedef struct
{
    Board *boards;
    uint16_t count;
} BoardStack;

BoardStack *create_board_stack(uint16_t size);
void destroy_board_stack(BoardStack *stack);
