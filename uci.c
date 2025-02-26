#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils/board.h"
#include "utils/fen.h"
#include "algorithm/game_history.h"
#include "utils/move.h"
#include "algorithm/bot.h"

#define UCI_LOG_FILE "uci_log.txt"
#define INPUT_BUFFER_SIZE 8192

Board current_board;
int move_overhead = 50;

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
    char *token = strtok(input, " "); // Should be "position"
    token = strtok(NULL, " ");        // Next token: "startpos" or "fen"

    if (!token)
        return;

    if (strcmp(token, "startpos") == 0)
    {
        new_game(STARTFEN);
        token = strtok(NULL, " ");
    }
    else if (strcmp(token, "fen") == 0)
    {
        // Build the FEN string from tokens until we hit "moves" or run out of tokens.
        char fen[256] = {0};
        bool firstField = true;
        while ((token = strtok(NULL, " ")) != NULL)
        {
            if (strcmp(token, "moves") == 0)
                break;
            if (!firstField)
                strcat(fen, " ");
            strcat(fen, token);
            firstField = false;
        }
        new_game(fen);
    }

    // If token equals "moves", process the move list.
    if (token != NULL && strcmp(token, "moves") == 0)
    {
        while ((token = strtok(NULL, " ")) != NULL)
        {
            do_move(token);
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

void log_uci(const char *format, ...)
{
    FILE *file = fopen(UCI_LOG_FILE, "a");
    if (file != NULL)
    {
        va_list args = NULL;
        va_start(args, format);
        vfprintf(file, format, args);
        fprintf(file, "\n");
        va_end(args);
        fclose(file);
    }
}

void log_board(Board board)
{
    FILE *file = fopen(UCI_LOG_FILE, "a");
    print_board_to_file(&board, file);
    fclose(file);
}

void respond(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // Print to stdout
    vprintf(format, args);
    printf("\n");
    fflush(stdout);

    // Log to file
    FILE *file = fopen(UCI_LOG_FILE, "a");
    if (file != NULL)
    {
        vfprintf(file, format, args);
        fprintf(file, "\n");
        fclose(file);
    }

    va_end(args);
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

void set_option(char *name, char *value)
{
    if (strcmp(name, "Move Overhead") == 0)
    {
        move_overhead = atoi(value);
        log_uci("Move Overhead set to %d", move_overhead);
    }
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
            respond("option name Move Overhead type spin default 50 min 0 max 1000");
            respond("option name Threads type spin default 1 min 1 max 1");

            respond("uciok");
        }
        else if (strcmp(input, "isready") == 0)
        {
            respond("readyok");
        }
        else if (strncmp(input, "setoption name ", 15) == 0)
        {
            char *name = strtok(input + 15, " ");
            char *value = strtok(NULL, " ");
            if (name && value)
            {
                set_option(name, value);
            }
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
            respond("bestmove %s", result.move);
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