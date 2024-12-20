#include "count_benchmark.h"

#include "../../fen.h"
#include <time.h>

uint64_t count_recursive(BoardState *board_state, uint8_t depth, BoardStack *stack)
{
    if (depth == 0)
        return 1;

    uint32_t base = stack->count;
    generate_moves(&board_state->board, stack);

    if (stack->count == base)
        return 0;

    uint64_t total = 0;
    for (uint16_t i = base; i < stack->count; i++)
    {
        total += count_recursive(&stack->boards[i], depth - 1, stack);
    }

    stack->count = base;
    return total;
}

void run_count_benchmark()
{
    BoardStack *stack = create_board_stack(65535);
    Board board = fen_to_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    BoardState board_state = board_to_board_state(&board);

    printf("%-7s %-13s %-11s %-17s %-17s\n", "Depth", "Nodes", "Time (s)", "Million boards/s", "Microseconds/board");
    for (uint8_t i = 1; i <= 7; i++)
    {
        clock_t start = clock();
        uint64_t result = count_recursive(&board_state, i, stack);
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        double million_boards_per_second = result / (time_spent * 1e6);
        double microseconds_per_board = (time_spent * 1e6) / result;
        printf("%-7u %-13llu %-11.3f %-17.3f %-17.3f\n", i, result, time_spent, million_boards_per_second, microseconds_per_board);
    }
}