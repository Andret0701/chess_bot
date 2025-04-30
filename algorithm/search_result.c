#include "search_result.h"

void print_search_result(SearchResult search_result)
{
    if (search_result.valid == VALID)
        print_score(search_result.board_score);
    else
        printf("Invalid\n");
}

void print_quiescence_result(QuiescenceResult quiescence_result)
{
    if (quiescence_result.valid == VALID)
        printf("Quiescence score: %d\n", quiescence_result.score);
    else
        printf("Invalid\n");
}