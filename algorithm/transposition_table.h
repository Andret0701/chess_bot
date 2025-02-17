#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "../utils/board.h"
#include "board_score.h"

typedef enum
{
    EXACT,
    NOT_EXACT,
} TT_Entry_Type;

typedef struct
{
    bool valid;
    Board board;
    BoardScore score;
    TT_Entry_Type type;
} TT_Entry;

void init_zobrist_keys();
void update_tt_entry(Board *board, BoardScore score, uint8_t depth, TT_Entry_Type type);
TT_Entry check_tt_entry(Board *board, uint8_t depth);
