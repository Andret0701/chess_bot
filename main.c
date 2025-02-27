#include <stdio.h>
#include "utils/board.h"
#include "engine/board_stack.h"
#include "engine/piece_moves.h"
#include "utils/fen.h"

#include "engine/tests/count_test.h"
#include "engine/benchmark/count_benchmark.h"
#include "algorithm/min_max.h"
#include "algorithm/board_score.h"

#include "algorithm/bot.h"
#include <string.h>
#include <stdlib.h>
#include "algorithm/game_history.h"
#include "utils/move.h"
#include "utils/profile.h"
#include "algorithm/known_endgames/endgames.h"
#include "engine/tests/can_move_test.h"
#include "engine/tests/capture_move_test.h"

#include "utils/bitboard.h"
#include "uci.h"

int main(int argc, char *argv[])
{
    // Board board = fen_to_board("r1bqkb1r/ppp2ppp/2n1pn2/3p4/3P4/PQN2N2/1PP1PPPP/R1B1KB1R w KQkq - 0 1");
    // print_board(&board);
    // // best move
    // BotResult result = run_bot((BotFlags){100000, 100000, 0, 0}, board);
    // print_bot_result(result);
    // exit(0);

    if (argc >= 2 && strcmp(argv[1], "profile") == 0)
    {
        play_game(30, 0.5);
        exit(0);
    }
    else if (argc >= 2 && strcmp(argv[1], "test") == 0)
    {
        printf("Running count tests\n");
        run_count_tests();
        printf("Running can move tests\n");
        run_can_move_tests();
        run_capture_move_tests();
        exit(0);
    }
    else if (argc >= 2 && strcmp(argv[1], "benchmark") == 0)
    {
        printf("Running benchmark\n");
        run_count_benchmark();
        exit(0);
    }

    uci_loop();
    return 0;
}
