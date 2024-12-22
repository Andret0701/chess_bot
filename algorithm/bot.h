#pragma once
#include "board_score.h"

typedef struct
{
    char *move;
    BoardScore score;
    uint8_t depth;
} BotResult;

BotResult run_bot(char *fen, double seconds);