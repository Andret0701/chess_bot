#include "search_result.h"

void print_search_result(SearchResult search_result)
{
    if (search_result.valid == VALID)
        print_score(search_result.board_score);
    else
        printf("Invalid\n");
}
