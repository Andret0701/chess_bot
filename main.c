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

#include "utils/bitboard.h"
#include "uci.h"
#include "algorithm/transposition_table.h"
#include "algorithm/zobrist_hash.h"

#include "algorithm/heuristic/heuristic.h"
#include "algorithm/heuristic/pawn_structure_score.h"
#include "engine/encoded_move.h"

int main(int argc, char *argv[])
{
    init_zobrist_keys();

    // Board board = fen_to_board(STARTFEN);
    // Board next_board = apply_move(&board, "e2e4");

    // uint16_t move = board_to_encoded_move(&board, &next_board);
    // print_move(move);
    // exit(0);

    // Board board = fen_to_board(STARTFEN);
    // BoardState board_state = board_to_board_state(&board);
    // print_board(&board);
    // get_pawn_structure_score(&board_state);
    // printf("\n");
    // board = fen_to_board("rnbqkbnr/pppppp2/6p1/8/7P/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1");
    // board_state = board_to_board_state(&board);
    // print_board(&board);
    // get_pawn_structure_score(&board_state);
    // printf("\n");
    // board = fen_to_board("rnbqkbnr/pppppp2/6p1/8/7P/8/PPPPPP2/RNBQKBNR b KQkq - 0 1");
    // board_state = board_to_board_state(&board);
    // print_board(&board);
    // get_pawn_structure_score(&board_state);
    // printf("\n");
    // exit(0);

    // for (int square = 0; square < 64; square++)
    // {
    //     uint64_t bitboard = position_to_bitboard(square % 8, square / 8);
    //     printf("Square: %d\n", square);
    //     print_bitboard(bitboard);
    //     printf("Backward pawn mask white:\n");
    //     print_bitboard(get_white_front_file_mask(bitboard));
    //     printf("Backward pawn mask black:\n");
    //     print_bitboard(get_black_front_file_mask(bitboard));
    // }
    // exit(0);

    // Board board = fen_to_board("3k1b1r/ppp1ppp1/6p1/6P1/8/P4n1q/1PP2P1B/7K b - -");
    // print_board(&board);
    // BotResult result = run_bot((BotFlags){30000, 30000, 0, 0}, board);
    // print_bot_result(result);
    // board = apply_move(&board, "f7f3");
    // print_board(&board);
    // result = run_bot((BotFlags){10000, 10000, 0, 0}, board);
    // print_bot_result(result);
    // print_board(&board);

    // for (int i = 0; i < 20; i++)
    // {
    //     board = apply_move(&board, result.move);
    //     print_bot_result(result);
    //     print_board(&board);
    // }
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
        printf("Running capture move tests\n");
        run_capture_move_tests();
        printf("Running encoded move tests\n");
        run_encoded_move_tests();
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
