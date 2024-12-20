#include <stdio.h>
#include "board.h"
#include "engine/board_stack.h"
#include "engine/piece_moves.h"
#include "fen.h"

#include "engine/tests/count_test.h"
#include "engine/benchmark/count_benchmark.h"

int main()
{
    run_count_benchmark();
    return 0;
}