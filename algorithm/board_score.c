#include "board_score.h"
#include <stdio.h>

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

bool is_better_result(Result a, Result b, Color color, Color bot_color)
{
    const Result white_scores[] = {WHITE_WON, UNKNOWN, DRAW, BLACK_WON};
    const Result black_scores[] = {BLACK_WON, UNKNOWN, DRAW, WHITE_WON};
    Result *scores = (color == WHITE) ? white_scores : black_scores;

    uint8_t a_index = 0;
    uint8_t b_index = 0;

    for (uint8_t i = 0; i < 4; i++)
    {
        if (a == scores[i])
            a_index = i;
        if (b == scores[i])
            b_index = i;
    }

    if (color == bot_color)
    {
        // If color is the bot's color, UNKNOWN is better than DRAW
        if (a == UNKNOWN && b == DRAW)
            return true;
        if (a == DRAW && b == UNKNOWN)
            return false;
    }
    else
    {
        // If color is not the bot's color, DRAW is better than UNKNOWN
        if (a == UNKNOWN && b == DRAW)
            return false;
        if (a == DRAW && b == UNKNOWN)
            return true;
    }

    return a_index < b_index;
}

bool is_better_score(BoardScore a, BoardScore b, Color color) // this should use depth. draw should be desired if loosing
{
    bool has_better_score = color == WHITE ? a.score >= b.score : a.score <= b.score;
    Result win_result = color == WHITE ? WHITE_WON : BLACK_WON;
    Result lost_result = color == WHITE ? BLACK_WON : WHITE_WON;

    if (a.result == win_result && b.result == win_result)
    {
        if (a.depth == b.depth)
            return has_better_score;
        return a.depth < b.depth;
    }
    else if (a.result == win_result && (b.result == UNKNOWN || b.result == DRAW || b.result == lost_result))
        return true;
    else if ((a.result == UNKNOWN || a.result == DRAW || a.result == lost_result) && b.result == win_result)
        return false;
    else if (a.result == UNKNOWN && b.result == UNKNOWN)
        return has_better_score;
    else if (a.result == UNKNOWN && b.result == DRAW)
    {
        bool a_is_winning = color == WHITE ? a.score >= 0 : a.score <= 0;
        if (a_is_winning)
            return true;
        return false;
    }
    else if (a.result == DRAW && b.result == UNKNOWN)
    {
        bool b_is_winning = color == WHITE ? b.score >= 0 : b.score <= 0;
        if (b_is_winning)
            return false;
        return true;
    }
    else if (a.result == UNKNOWN && b.result == lost_result)
        return true;
    else if (a.result == lost_result && b.result == UNKNOWN)
        return false;
    else if (a.result == DRAW && b.result == DRAW)
    {
        bool a_is_loosing = color == WHITE ? a.score < 0 : a.score > 0;
        bool b_is_loosing = color == WHITE ? b.score < 0 : b.score > 0;
        bool both_is_loosing = a_is_loosing && b_is_loosing;
        if (both_is_loosing && a.depth < b.depth)
            return true;

        bool a_is_winning = color == WHITE ? a.score >= 0 : a.score <= 0;
        bool b_is_winning = color == WHITE ? b.score >= 0 : b.score <= 0;
        bool both_is_winning = a_is_winning && b_is_winning;
        if (both_is_winning && a.depth > b.depth)
            return true;

        return has_better_score;
    }
    else if (a.result == DRAW && b.result == lost_result)
        return true;
    else if (a.result == lost_result && b.result == DRAW)
        return false;

    // else if (a.result == lost_result && b.result == lost_result)
    // {
    if (a.depth == b.depth)
        return has_better_score;
    return a.depth > b.depth;
    // }

    // w-w w-u w-d w-l
    // u-w u-u u-d u-l
    // d-w d-u d-d d-l
    // l-w l-u l-d l-l
}

bool is_better_equal(BoardScore a, BoardScore b, Color color, Color bot_color)
{
    if (a.result == b.result && a.score == b.score && a.depth == b.depth)
        return true;

    return is_better_score(a, b, color, bot_color);
}

BoardScore max_score(BoardScore a, BoardScore b, Color color, Color bot_color)
{
    if (is_better_score(a, b, color, bot_color))
        return a;
    return b;
}

void print_score(BoardScore score)
{
    const char *results[] = {"White won", "Black won", "Draw", "Unknown"};
    printf("Score: %d, Result: %s, Depth: %d\n", score.score, results[score.result], score.depth);
}