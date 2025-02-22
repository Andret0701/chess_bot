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

#include "utils/bitboard.h"

Board current_board;
void new_game()
{
    current_board = fen_to_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    reset_game_history();
    push_game_history(current_board);
}

int main(int argc, char *argv[])
{

    if (argc >= 2 && strcmp(argv[1], "profile") == 0)
    {
        // play_game(0.1);
        //  play_game(0.2);
        play_game(0.3);
        play_game(0.4);
        play_game(1.0);
        // play_game(2.0);
        // play_game(3.0);
        exit(0);
    }
    else if (argc >= 2 && strcmp(argv[1], "test") == 0)
    {
        printf("Running count tests\n");
        run_count_tests();
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

    char command[256];
    fflush(stdout); // Ensure output is sent immediately

    new_game();
    while (1)
    {
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break; // Exit if no input (EOF)
        }

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "test") == 0)
        {
            printf("Running tests\n");
            run_count_tests();
        }
        else if (strcmp(command, "benchmark") == 0)
        {
            printf("Running benchmark\n");
            run_count_benchmark();
        }
        else if (strncmp(command, "bot", 3) == 0)
        {
            // Example: Process "bot <fen> <seconds>"
            char fen[128];
            double seconds;
            sscanf(command + 4, "\"%127[^\"]\" %lf", fen, &seconds);
            BotResult result = run_bot(fen, seconds);
            current_board = fen_to_board(fen);

            printf("%s\n", result.move);        // Move in UCI format
            printf("%d\n", result.score.score); // Score of the position
            printf("%d\n", result.score.depth); // Search depth
            if (result.score.result == WHITE_WON)
                printf("White won\n");
            else if (result.score.result == BLACK_WON)
                printf("Black won\n");
            else if (result.score.result == DRAW)
                printf("Draw\n");
            else
                printf("Unknown\n");
        }
        else if (strcmp(command, "new") == 0)
        {
            new_game();
        }
        else if (strcmp(command, "print") == 0)
        {
            print_board(&current_board);
        }
        else if (strncmp(command, "move", 4) == 0)
        {
            // Example: Process "move e2e4"
            char move[6];
            memset(move, 0, sizeof(move));
            sscanf(command + 5, "%5s", move);
            if (can_move(&current_board, move))
            {
                current_board = apply_move(&current_board, move);
                push_game_history(current_board);
            }
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else
        {
            printf("Unknown command: %s\n", command);
        }
        fflush(stdout);
    }

    return 0;
}
