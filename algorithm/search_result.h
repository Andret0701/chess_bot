#pragma once

#include "board_score.h"

typedef enum
{
    INVALID,
    VALID
} Validity;

typedef struct
{
    BoardScore board_score;
    Validity valid;
} SearchResult;

typedef struct
{
    int32_t score;
    Validity valid;
} QuiescenceResult;

void print_search_result(SearchResult search_result);
void print_quiescence_result(QuiescenceResult quiescence_result);
