#include "count_benchmark.h"
#include "../../utils/fen.h"
#include <stdio.h>
#include <stdlib.h>
#include "../piece_moves.h"
#include "../tests/count_tests.h"
#include "../../algorithm/bot.h"
#include <inttypes.h>

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

    uint64_t total_nodes = 0;
    uint64_t total_squares = 0;
    for (size_t i = 0; i < num_tests; i++)
    {
        BoardState board_state = board_to_board_state(&boards[i]);
        BotResult result = run_depth_bot(boards[i], 5);
        total_nodes += result.nodes_searched;
        total_squares += result.nodes_searched * result.nodes_searched;
    }
    printf("Total nodes searched: %" PRIu64 "\n", total_nodes);
    printf("Average nodes per position: %.2f\n", (double)total_nodes / num_tests);
    printf("Total nodes squared: %" PRIu64 "\n", total_squares);
    printf("Average nodes squared per position: %.2f\n", (double)total_squares / num_tests);

    free(boards);
}