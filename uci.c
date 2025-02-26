#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include "utils/board.h"
#include "utils/fen.h"
#include "algorithm/game_history.h"
#include "utils/move.h"
#include "algorithm/bot.h"

#define UCI_LOG_FILE "uci_log.txt"
#define INPUT_BUFFER_SIZE 4096

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

void log_uci(char *message)
{
    // a file named uci_log.txt will be created in the same directory as the executable
    FILE *file = fopen(UCI_LOG_FILE, "a");
    fprintf(file, "%s\n", message);
    fclose(file);
}

void log_board(Board board)
{
    FILE *file = fopen(UCI_LOG_FILE, "a");
    print_board_to_file(&board, file);
    fclose(file);
}

void respond(char *response)
{
    printf("%s\n", response);
    fflush(stdout);
    log_uci(response);
}

// get message
void listen(char *message)
{
    while (fgets(message, INPUT_BUFFER_SIZE, stdin) == NULL)
        ;
    // Remove newline
    message[strcspn(message, "\n")] = 0;
    log_uci(message);
}

void uci_loop()
{
    new_game(STARTFEN);
    char input[INPUT_BUFFER_SIZE];

    while (1)
    {
        listen(input);
        if (strcmp(input, "uci") == 0)
        {
            // Engine identification
            respond("id name AndoBot");
            respond("id author Andreas Tolstrup Christensen");
            respond("uciok");
        }
        else if (strcmp(input, "isready") == 0)
        {
            respond("readyok");
        }
        else if (strcmp(input, "ucinewgame") == 0)
        {
            // Start a new game
            new_game(STARTFEN);
            log_board(current_board);
        }
        else if (strncmp(input, "position", 8) == 0)
        {
            // Handle setting up the board position
            parse_position(input);
            log_board(current_board);
        }
        else if (strncmp(input, "go", 2) == 0)
        {
            // Instead of searching, return a dummy move
            BotFlags flags = parse_go(input);
            BotResult result = run_bot(flags, current_board);
            respond(result.move);
        }
        else if (strcmp(input, "quit") == 0)
        {
            break;
        }
        else
            log_uci("Unknown command");

        log_uci("");
    }
}