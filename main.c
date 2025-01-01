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
#include "algorithm/game_history.h"
#include "move.h"
#include "profile.h"
#include "algorithm/known_endgames/endgames.h"

Board current_board;
void new_game()
{
    current_board = fen_to_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    reset_game_history();
    push_game_history(current_board);
}

int main(int argc, char *argv[])
{
    // init_endgames();

    // // 8/8/1r6/8/8/2k5/8/K7 w - - 0 1, -2
    // Board empty_board = fen_to_board("8/8/8/8/8/2k5/P7/K7 w - - 0 1");
    // BoardState empty_board_state = board_to_board_state(&empty_board);
    // BoardScore endscore = score_endgame(&empty_board_state);

    // printf("%d\n", endscore.score); // Score of the position
    // printf("%d\n", endscore.depth); // Search depth
    // if (endscore.result == WHITE_WON)
    //     printf("White won\n");
    // else if (endscore.result == BLACK_WON)
    //     printf("Black won\n");
    // else if (endscore.result == DRAW)
    //     printf("Draw\n");
    // else
    //     printf("Unknown\n");

    // free_endgames();

    if (argc >= 2 && strcmp(argv[1], "profile") == 0)
    {
        play_game(1.0);
        play_game(2.0);
        play_game(3.0);
        exit(0);
    }
    else if (argc >= 2 && strcmp(argv[1], "test") == 0)
    {
        printf("Running tests\n");
        run_count_tests();
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
