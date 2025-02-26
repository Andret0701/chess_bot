#pragma once
#include "board_score.h"

typedef struct
{
    char *move;
    BoardScore score;
    uint8_t depth;
} BotResult;

typedef struct
{
    int wtime;     // Time left for white in milliseconds
    int btime;     // Time left for black in milliseconds
    int winc;      // Time increment for white in milliseconds
    int binc;      // Time increment for black in milliseconds
    int movestogo; // Moves left to the next time control
} BotFlags;

void print_bot_result(BotResult result);
BotResult run_bot(BotFlags flags, Board board);