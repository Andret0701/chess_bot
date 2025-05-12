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

void print_search_result(SearchResult search_result);