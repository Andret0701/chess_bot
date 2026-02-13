#include "board_score.h"
#include <stdio.h>
#include <limits.h>
#include <inttypes.h>
#include "algorithm/heuristic/heuristic_values.h"

bool is_draw(Result result)
{
    return result == STALEMATE || result == THREEFOLD_REPETITION || result == FIFTY_MOVE_RULE || result == INSUFFICIENT_MATERIAL;
}

BoardScore invert_score(BoardScore score)
{
    BoardScore inverted_score;
    inverted_score.score = -score.score;
    inverted_score.result = score.result == WON ? LOST : (score.result == LOST ? WON : score.result);
    inverted_score.depth = score.depth;
    return inverted_score;
}

const char *result_to_string(Result result)
{
    switch (result)
    {
    case WON:
        return "Won";
    case LOST:
        return "Lost";
    case STALEMATE:
        return "Stalemate";
    case THREEFOLD_REPETITION:
        return "Threefold repetition";
    case FIFTY_MOVE_RULE:
        return "Fifty move rule";
    case INSUFFICIENT_MATERIAL:
        return "Insufficient material";
    case UNKNOWN:
        return "Unknown";
    default:
        return "Invalid result";
    }
}

void print_score(BoardScore score)
{
    double score_value = ((double)score.score) / ((double)HEURISTIC_SCALE * 24);
    printf("Score: %.2f, Result: %s, Depth: %d\n", score_value, result_to_string(score.result), score.depth);
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

bool is_less_equal_score(BoardScore a, BoardScore b)
{
    if (is_equal_score(a, b))
        return true;

    return is_less_score(a, b);
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