#include "uci.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils/board.h"
#include "utils/fen.h"
#include "algorithm/game_history.h"
#include "utils/move.h"
#include "algorithm/bot.h"
#include "algorithm/zobrist_hash.h"
#include "algorithm/heuristic/heuristic_values.h"

#define IS_JUNIOR false
#define UCI_LOG_FILE "uci_log.txt"
#define INPUT_BUFFER_SIZE 8192
#define LOG_UCI false

Board previous_board;
Board current_board;
int move_overhead = 50;

void new_game(char *input)
{
    current_board = fen_to_board(input);
    previous_board = current_board;
    reset_game_history();

    uint64_t hash = hash_board(&current_board);
    push_game_history(hash);
}

void do_move(char *move)
{
    if (can_move(&current_board, move))
    {
        previous_board = current_board;
        current_board = apply_move(&current_board, move);
        force_push_game_history(&previous_board, &current_board);
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

UCIGoFlags parse_go(char *input)
{
    UCIGoFlags flags = {0};

    char *token = strtok(input, " ");
    token = strtok(NULL, " ");

    while (token != NULL)
    {
        if (strcmp(token, "wtime") == 0)
        {
            token = strtok(NULL, " ");
            flags.wtime = atoi(token);
            flags.search_option = BOT_SEARCH_TIME;
        }
        else if (strcmp(token, "btime") == 0)
        {
            token = strtok(NULL, " ");
            flags.btime = atoi(token);
            flags.search_option = BOT_SEARCH_TIME;
        }
        else if (strcmp(token, "winc") == 0)
        {
            token = strtok(NULL, " ");
            flags.winc = atoi(token);
            flags.search_option = BOT_SEARCH_TIME;
        }
        else if (strcmp(token, "binc") == 0)
        {
            token = strtok(NULL, " ");
            flags.binc = atoi(token);
            flags.search_option = BOT_SEARCH_TIME;
        }
        else if (strcmp(token, "depth") == 0)
        {
            token = strtok(NULL, " ");
            flags.depth = atoi(token);
            flags.search_option = BOT_SEARCH_DEPTH;
        }
        else if (strcmp(token, "movetime") == 0)
        {
            token = strtok(NULL, " ");
            flags.movetime = atoi(token);
            flags.search_option = BOT_SEARCH_MOVETIME;
        }
        else if (strcmp(token, "nodes") == 0)
        {
            token = strtok(NULL, " ");
            flags.nodes = strtoull(token, NULL, 10);
            flags.search_option = BOT_SEARCH_NODES;
        }
        else if (strcmp(token, "heuristic") == 0)
        {
            flags.search_option = BOT_SEARCH_HEURISTIC;
        }

        token = strtok(NULL, " ");
    }

    return flags;
}

void log_uci(const char *format, ...)
{
    if (!LOG_UCI)
        return;

    if (format == NULL)
        return; // guard against NULL format

    FILE *file = fopen(UCI_LOG_FILE, "a");
    if (!file)
        return;

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    fputc('\n', file);
    va_end(args);

    fclose(file);
}

void log_board(Board board)
{
    if (!LOG_UCI)
        return;

    FILE *file = fopen(UCI_LOG_FILE, "a");
    print_board_to_file(&board, file);
    fclose(file);
}

void respond(const char *format, ...)
{
    if (format == NULL)
        return; // guard against NULL format

    va_list args;
    va_start(args, format);

    // Print to stdout
    vprintf(format, args);
    printf("\n");
    fflush(stdout);

    va_list copy;
    va_copy(copy, args);

    // Log to file
    if (LOG_UCI)
    {
        FILE *file = fopen(UCI_LOG_FILE, "a");
        if (file)
        {
            vfprintf(file, format, copy);
            fputc('\n', file);
            fclose(file);
        }
    }

    va_end(copy);
    va_end(args);
}

// get message
void listen_uci(char *message)
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

void uci_loop(bool debug_mode)
{
    new_game(STARTFEN);
    char input[INPUT_BUFFER_SIZE];

    while (1)
    {
        listen_uci(input);
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
            UCIGoFlags flags = parse_go(input);
            BotResult result;
            if (IS_JUNIOR)
                result = run_depth_bot(current_board, 3);
            else
            {
                switch (flags.search_option)
                {
                case BOT_SEARCH_TIME:
                    result = run_time_bot(current_board, flags.wtime, flags.btime, flags.winc, flags.binc);
                    break;
                case BOT_SEARCH_DEPTH:
                    result = run_depth_bot(current_board, flags.depth);
                    break;
                case BOT_SEARCH_MOVETIME:
                    result = run_movetime_bot(current_board, flags.movetime);
                    break;
                case BOT_SEARCH_NODES:
                    result = run_nodes_bot(current_board, flags.nodes);
                    break;
                case BOT_SEARCH_HEURISTIC:
                    result = run_heuristic_bot(current_board);
                    break;
                }
            }

            if (debug_mode)
            {
                double score_value = ((double)result.score) / ((double)HEURISTIC_SCALE * 24);
                respond("bestmove %s score %.2f depth %d", result.move, score_value, result.depth);
            }
            else
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