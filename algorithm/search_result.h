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
    double score;
    Validity valid;
} QuiescenceResult;

void print_search_result(SearchResult search_result);
void print_quiescence_result(QuiescenceResult quiescence_result);
