#pragma once

#include <stdint.h>
#include "../../board.h"
#include "../board_stack.h"

typedef struct
{
    const char *fen;
    uint8_t depth;
    uint64_t expected;
} Test;

uint64_t count_recursive(Board *board, uint8_t depth, BoardStack *stack);
void run_count_tests(Test *tests, uint8_t count);