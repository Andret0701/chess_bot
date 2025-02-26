#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include "utils/board.h"
#include "utils/fen.h"
#include "algorithm/game_history.h"
#include "utils/move.h"
#include "algorithm/bot.h"

Board current_board;
void new_game(char *input)
{
    current_board = fen_to_board(input);
    reset_game_history();
    push_game_history(current_board);
}

void do_move(char *move)
{
    if (can_move(&current_board, move))
    {
        current_board = apply_move(&current_board, move);
        push_game_history(current_board);
    }
}

void parse_position(char *input)
{
    char *token = strtok(input, " ");
    token = strtok(NULL, " ");

    if (strcmp(token, "startpos") == 0)
    {
        new_game(STARTFEN);
        token = strtok(NULL, " ");
    }
    else if (strcmp(token, "fen") == 0)
    {
        new_game(token);
        token = strtok(NULL, " ");
    }

    if (strcmp(token, "moves") == 0)
    {
        token = strtok(NULL, " ");
        while (token != NULL)
        {
            do_move(token);
            token = strtok(NULL, " ");
        }
    }
}

BotFlags parse_go(char *input)
{
    BotFlags flags = {0};

    char *token = strtok(input, " ");
    token = strtok(NULL, " ");

    while (token != NULL)
    {
        if (strcmp(token, "wtime") == 0)
        {
            token = strtok(NULL, " ");
            flags.wtime = atoi(token);
        }
        else if (strcmp(token, "btime") == 0)
        {
            token = strtok(NULL, " ");
            flags.btime = atoi(token);
        }
        else if (strcmp(token, "winc") == 0)
        {
            token = strtok(NULL, " ");
            flags.winc = atoi(token);
        }
        else if (strcmp(token, "binc") == 0)
        {
            token = strtok(NULL, " ");
            flags.binc = atoi(token);
        }
        else if (strcmp(token, "movestogo") == 0)
        {
            token = strtok(NULL, " ");
            flags.movestogo = atoi(token);
        }

        token = strtok(NULL, " ");
    }

    return flags;
}

void uci_loop()
{
    new_game(STARTFEN);
    char input[4096];

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            continue;

        // Remove newline
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "uci") == 0)
        {
            // Engine identification
            printf("id name AndoBot\n");
            printf("id author Andreas Tolstrup Christensen\n");
            printf("uciok\n");
            fflush(stdout);
        }
        else if (strcmp(input, "isready") == 0)
        {
            printf("readyok\n");
            fflush(stdout);
        }
        else if (strcmp(input, "ucinewgame") == 0)
        {
            // Start a new game
            new_game(STARTFEN);
        }
        else if (strncmp(input, "position", 8) == 0)
        {
            // Handle setting up the board position
            parse_position(input);
        }
        else if (strncmp(input, "go", 2) == 0)
        {
            // Instead of searching, return a dummy move
            BotFlags flags = parse_go(input);
            BotResult result = run_bot(flags, current_board);
            printf("bestmove %s\n", result.move);
            fflush(stdout);
        }
        else if (strcmp(input, "quit") == 0)
        {
            break;
        }
    }
}