#include "board_score.h"
#include <stdio.h>
#include <limits.h>

BoardScore invert_score(BoardScore score)
{
    BoardScore inverted_score;
    inverted_score.score = -score.score;
    inverted_score.result = score.result == WON ? LOST : (score.result == LOST ? WON : score.result);
    inverted_score.depth = score.depth;
    return inverted_score;
}

void print_score(BoardScore score)
{
    const char *results[] = {"Won", "Lost", "Draw", "Unknown"};
    printf("Score: %.2f, Result: %s, Depth: %d\n", score.score, results[score.result], score.depth);
}

bool is_greater_score(BoardScore a, BoardScore b)
{
    // Results are ordered as follows:
    // WON > UNKNOWN / DRAW > LOST
    if (a.result == WON && b.result != WON)
        return true;
    else if (a.result != WON && b.result == WON)
        return false;
    else if (a.result == LOST && b.result != LOST)
        return false;
    else if (a.result != LOST && b.result == LOST)
        return true;

    if (a.result == WON && b.result == WON)
        return a.depth < b.depth;
    else if (a.result == LOST && b.result == LOST)
        return a.depth > b.depth;

    // Assumes DRAWs score is 0
    return a.score > b.score;
}

bool is_equal_score(BoardScore a, BoardScore b)
{
    return !is_greater_score(a, b) && !is_greater_score(b, a);
}

bool is_greater_equal_score(BoardScore a, BoardScore b)
{
    if (is_equal_score(a, b))
        return true;

    return is_greater_score(a, b);
}

bool is_less_score(BoardScore a, BoardScore b)
{
    return !is_greater_equal_score(a, b);
}

BoardScore max_score(BoardScore a, BoardScore b)
{
    if (is_greater_score(a, b))
        return a;
    return b;
}

BoardScore min_score(BoardScore a, BoardScore b)
{
    if (is_greater_score(a, b))
        return b;
    return a;
}