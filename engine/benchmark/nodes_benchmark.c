#include "count_benchmark.h"
#include "../../utils/fen.h"
#include <stdio.h>
#include <stdlib.h>
#include "../piece_moves.h"
#include "../tests/count_tests.h"
#include "../../algorithm/bot.h"

void run_nodes_benchmark()
{
    const size_t num_tests = sizeof(tests) / sizeof(Test);

    Board *boards = malloc(num_tests * sizeof(Board));
    if (!boards)
    {
        fprintf(stderr, "Failed to allocate boards array\n");
        return;
    }

    // Fill in all positions
    for (size_t i = 0; i < num_tests; i++)
    {
        boards[i] = fen_to_board(tests[i].fen);
    }

    double average_nodes = 0;
    for (size_t i = 0; i < num_tests; i++)
    {
        BoardState board_state = board_to_board_state(&boards[i]);
        BotResult result = run_depth_bot(boards[i], 5);
        average_nodes += result.nodes_searched * (1.0 / num_tests);
    }
    printf("Average nodes searched: %.2f\n", average_nodes);

    free(boards);
}