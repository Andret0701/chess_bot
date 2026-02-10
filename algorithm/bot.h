#pragma once
#include "board_score.h"
#include "../engine/board_state.h"

typedef struct
{
    BoardState *board;
    BoardScore score;
} BotMove;

typedef struct
{
    char *move;
    BoardScore score;
    uint8_t depth;
} BotResult;

void print_bot_result(BotResult result);
BotResult run_depth_bot(Board board, uint8_t depth);
BotResult run_time_bot(Board board, int wtime, int btime, int winc, int binc);
BotResult run_movetime_bot(Board board, int movetime);
BotResult run_nodes_bot(Board board, uint64_t nodes);
BotResult run_heuristic_bot(Board board);