#include "board_score.h"
#include <stdio.h>
#include <limits.h>

// BoardScore invert_score(BoardScore score);
// void print_score(BoardScore score);

// bool is_greater_score(BoardScore a, BoardScore b);
// bool is_equal_score(BoardScore a, BoardScore b);
// bool is_greater_equal_score(BoardScore a, BoardScore b);

// BoardScore max_score(BoardScore a, BoardScore b);
// BoardScore min_score(BoardScore a, BoardScore b);

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
    printf("Score: %d, Result: %s, Depth: %d\n", score.score, results[score.result], score.depth);
}

bool is_greater_score(BoardScore a, BoardScore b)
{
    bool has_better_score = a.score > b.score;
    if (a.result == WON && b.result == WON)
    {
        if (a.depth == b.depth)
            return has_better_score;
        return a.depth < b.depth;
    }
    else if (a.result == WON && (b.result == UNKNOWN || b.result == DRAW || b.result == LOST))
        return true;
    else if ((a.result == UNKNOWN || a.result == DRAW || a.result == LOST) && b.result == WON)
        return false;
    else if (a.result == UNKNOWN && b.result == UNKNOWN)
        return has_better_score;
    else if (a.result == UNKNOWN && b.result == DRAW)
    {
        bool a_is_winning = a.score >= 0;
        if (a_is_winning)
            return true;
        return false;
    }
    else if (a.result == DRAW && b.result == UNKNOWN)
    {
        bool b_is_winning = b.score >= 0;
        if (b_is_winning)
            return false;
        return true;
    }
    else if (a.result == UNKNOWN && b.result == LOST)
        return true;
    else if (a.result == LOST && b.result == UNKNOWN)
        return false;
    else if (a.result == DRAW && b.result == DRAW)
    {
        bool a_is_loosing = a.score < 0;
        bool b_is_loosing = b.score < 0;
        bool both_is_loosing = a_is_loosing && b_is_loosing;
        if (both_is_loosing && a.depth < b.depth)
            return true;

        bool a_is_winning = a.score >= 0;
        bool b_is_winning = b.score >= 0;
        bool both_is_winning = a_is_winning && b_is_winning;
        if (both_is_winning && a.depth > b.depth)
            return true;

        return has_better_score;
    }
    else if (a.result == DRAW && b.result == LOST)
        return true;
    else if (a.result == LOST && b.result == DRAW)
        return false;

    if (a.depth == b.depth)
        return has_better_score;
    return a.depth > b.depth;
}

bool is_equal_score(BoardScore a, BoardScore b)
{
    return a.result == b.result && a.score == b.score && a.depth == b.depth;
}

bool is_greater_equal_score(BoardScore a, BoardScore b)
{
    if (is_equal_score(a, b))
        return true;

    return is_greater_score(a, b);
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