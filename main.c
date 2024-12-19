#include <stdio.h>
#include "board.h"
#include "engine/board_stack.h"
#include "engine/piece_moves.h"
#include "fen.h"

#include "engine/tests/count_test.h"

int main()
{
    run_count_tests();
    return 0;
}