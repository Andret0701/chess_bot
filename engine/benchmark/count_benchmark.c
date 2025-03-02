#include "count_benchmark.h"

#include "../../utils/fen.h"
#include <time.h>
#include <stdio.h>
#include "../piece_moves.h"

uint64_t count_recursive(BoardState *board_state, uint8_t depth, BoardStack *stack)
{
    if (depth == 0)
        return 1;

    uint32_t base = stack->count;
    generate_moves(board_state, stack);

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

// Benchmark results v1:
// Depth   Nodes         Time (s)    Million boards/s  Microseconds/board
// 1       20            0.000       inf               0.000
// 2       400           0.001       0.400             2.500
// 3       8902          0.002       4.451             0.225
// 4       197281        0.062       3.182             0.314
// 5       4865609       1.290       3.772             0.265
// 6       119060324     32.686      3.643             0.275

// Benchmark results v2 (Added magic bitboards):
// Depth   Nodes         Time (s)    Million boards/s  Microseconds/board
// 1       20            0.000       inf               0.000
// 2       400           0.000       1.600             0.625
// 3       8902          0.001       8.902             0.112
// 4       197281        0.028       7.109             0.141
// 5       4865609       0.664       7.322             0.137
// 6       119060324     16.248      7.328             0.136

// Benchmark results v3 (Improved attack generation):
// Depth   Nodes         Time (s)    Million boards/s  Microseconds/board
// 1       20            0.000       inf               0.000
// 2       400           0.000       1.600             0.625
// 3       8902          0.001       11.869            0.084
// 4       197281        0.022       8.768             0.114
// 5       4865609       0.537       9.065             0.110
// 6       119060324     13.611      8.748             0.114

// Benchmark results v4 (Improved attack generation):
// Depth   Nodes         Time (s)    Million boards/s  Microseconds/board
// 1       20            0.000       inf               0.000
// 2       400           0.000       inf               0.000
// 3       8902          0.000       inf               0.000
// 4       197281        0.014       14.091            0.071
// 5       4865609       0.347       14.022            0.071
// 6       119060324     8.449       14.092            0.071

void run_count_benchmark()
{
    BoardStack *stack = create_board_stack(65535);
    Board board = fen_to_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    BoardState board_state = board_to_board_state(&board);

    printf("%-7s %-13s %-11s %-17s %-17s\n", "Depth", "Nodes", "Time (s)", "Million boards/s", "Microseconds/board");
    for (uint8_t i = 1; i <= 6; i++)
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