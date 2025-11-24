#include <stdio.h>
#include "utils/board.h"
#include "engine/board_stack.h"
#include "engine/piece_moves.h"
#include "utils/fen.h"

#include "engine/tests/count_test.h"
#include "engine/benchmark/count_benchmark.h"
#include "algorithm/board_score.h"

#include "algorithm/bot.h"
#include <string.h>
#include <stdlib.h>
#include "algorithm/game_history.h"
#include "utils/move.h"
#include "utils/profile.h"
#include "engine/tests/can_move_test.h"
#include "engine/tests/capture_move_test.h"
#include "engine/tests/encoded_move_test.h"
#include "engine/tests/attack_test.h"
#include "algorithm/heuristic/tests/symetric_heuristic_test.h"

#include "utils/bitboard.h"
#include "uci.h"
#include "algorithm/transposition_table.h"
#include "algorithm/zobrist_hash.h"

#include "algorithm/heuristic/heuristic.h"
#include "algorithm/heuristic/pawn_structure_score.h"
#include "engine/encoded_move.h"
#include "algorithm/reductions.h"
#include "algorithm/heuristic/heuristic_values.h"

int main(int argc, char *argv[])
{
    init_zobrist_keys();
    init_reductions();

    if (argc >= 2 && strcmp(argv[1], "profile") == 0)
    {
        play_game(3, 0.05);
        play_game(5, 0.1);
        play_game(10, 0.2);
        play_game(30, 0.5);
        exit(0);
    }
    else if (argc >= 2 && strcmp(argv[1], "test") == 0)
    {
        printf("Running attack tests\n");
        run_attack_tests();
        printf("Running count tests\n");
        run_count_tests();
        printf("Running encoded move tests\n");
        run_encoded_move_tests();
        printf("Running capture move tests\n");
        run_capture_move_tests();
        printf("Running heuristic tests\n");
        run_heuristic_eval_tests();
        printf("Running can move tests\n");
        run_can_move_tests();
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
