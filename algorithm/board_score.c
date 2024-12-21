#include "board_score.h"

bool has_won(Result result, Color color)
{
    return (result == WHITE_WON && color == WHITE) || (result == BLACK_WON && color == BLACK);
}

bool has_lost(Result result, Color color)
{
    return (result == BLACK_WON && color == WHITE) || (result == WHITE_WON && color == BLACK);
}

BoardScore get_worst_score(Color color)
{
    BoardScore score;
    score.score = color == WHITE ? INT32_MIN : INT32_MAX;
    score.result = color == WHITE ? BLACK_WON : WHITE_WON;
    score.depth = 0;
    return score;
}

bool is_better_score(BoardScore a, BoardScore b, Color color)
{
    if (color == WHITE)
    {
        if (a.result == b.result)
            return a.score > b.score;

        if (a.result == WHITE_WON)
            return true;

        if (b.result == WHITE_WON)
            return false;

        if (a.result == UNKNOWN)
            return true;

        if (b.result == UNKNOWN)
            return false;

        if (a.result == DRAW)
            return true;

        return false;
    }
    else
    {
        if (a.result == b.result)
            return a.score < b.score;

        if (a.result == BLACK_WON)
            return true;

        if (b.result == BLACK_WON)
            return false;

        if (a.result == UNKNOWN)
            return true;

        if (b.result == UNKNOWN)
            return false;

        if (a.result == DRAW)
            return true;

        return false;
    }
}

bool is_better_equal(BoardScore a, BoardScore b, Color color)
{
    if (color == WHITE)
    {
        if (a.result == b.result)
            return a.score >= b.score;

        if (a.result == WHITE_WON)
            return true;

        if (b.result == WHITE_WON)
            return false;

        if (a.result == UNKNOWN)
            return true;

        if (b.result == UNKNOWN)
            return false;

        if (a.result == DRAW)
            return true;

        return false;
    }
    else
    {
        if (a.result == b.result)
            return a.score <= b.score;

        if (a.result == BLACK_WON)
            return true;

        if (b.result == BLACK_WON)
            return false;

        if (a.result == UNKNOWN)
            return true;

        if (b.result == UNKNOWN)
            return false;

        if (a.result == DRAW)
            return true;

        return false;
    }
}

BoardScore max_score(BoardScore a, BoardScore b, Color color)
{
    if (is_better_score(a, b, color))
        return a;
    return b;
}

void print_score(BoardScore score)
{
    const char *results[] = {"White won", "Black won", "Draw", "Unknown"};
    printf("Score: %d, Result: %s, Depth: %d\n", score.score, results[score.result], score.depth);
}