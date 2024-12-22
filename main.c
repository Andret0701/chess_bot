#include <stdio.h>
#include "board.h"
#include "engine/board_stack.h"
#include "engine/piece_moves.h"
#include "fen.h"

#include "engine/tests/count_test.h"
#include "engine/benchmark/count_benchmark.h"
#include "algorithm/min_max.h"
#include "algorithm/board_score.h"

#include "algorithm/bot.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[] )
{
    if (argc == 1)
    {
        printf("Usage: %s test|benchmark|bot <fen> <seconds>\n", argv[0]);
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "test") == 0)
    {
        run_count_tests();
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "benchmark") == 0)
    {
        run_count_benchmark();
        return 0;
    }

    if (argc > 3 && strcmp(argv[1], "bot") == 0)
    {
        char *fen = argv[2];
        double seconds = atof(argv[3]);
        BotResult bot_result = run_bot(fen, seconds);
        printf("%s\n", bot_result.move);
        printf("%d\n", bot_result.score.score);
        printf("%d\n", bot_result.depth);
        if (has_won(bot_result.score.result, WHITE))
            printf("White won\n");
        else if (has_won(bot_result.score.result, BLACK))
            printf("Black won\n");
        else if (bot_result.score.result == DRAW)
            printf("Draw\n");
        else
            printf("Unknown\n");
        return 0;
    }



    return 0;
}