#include "profile.h"

#include "engine/board_stack.h"
#include "algorithm/bot.h"
#include "fen.h"
#include <time.h>
#include <stdio.h>
#include "algorithm/min_max.h"
#include "move.h"
#include "algorithm/game_history.h"
#include "algorithm/heuristic.h"

void play_game(double think_time)
{
    Board board = fen_to_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    push_game_history(board);
    BoardState board_state = board_to_board_state(&board);
    reset_game_history();
    while (true)
    {
        // print_board(&board_state.board);
        clock_t start = clock();
        BotResult result = run_bot(board_to_fen(&board_state.board), think_time);
        clock_t end = clock();
        double time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (time_used > think_time)
        {
            printf("Used to long to think: %f\n", think_time);
            printf("That was %f seconds to much.\n", time_used - think_time);
        }

        printf("Move: %s\n", result.move);
        // printf("Score: %d\n", result.score.score);
        // printf("Depth: %d\n", result.score.depth);
        // if (result.score.result == WHITE_WON)
        //     printf("White won\n");
        // else if (result.score.result == BLACK_WON)
        //     printf("Black won\n");
        // else if (result.score.result == DRAW)
        //     printf("Draw\n");
        // else
        //     printf("Unknown\n");

        if (result.score.result != UNKNOWN)
            break;

        board = apply_move(&board, result.move);
        push_game_history(board);
        board_state = board_to_board_state(&board);

        if (threefold_repetition())
        {
            printf("Draw by threefold repetition\n");
            break;
        }

        if (has_insufficient_material(&board))
        {
            printf("Draw by insufficient material\n");
            break;
        }

        if (get_game_result(&board_state) != UNKNOWN)
            break;

        // printf("\n");
    }

    print_board(&board);
}