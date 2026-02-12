#include "game_history.h"
#include <stdlib.h>

#define MAX_GAME_HISTORY 20000

uint64_t game_history[MAX_GAME_HISTORY] = {0};
uint16_t move_count = 0;

void reset_game_history()
{
    move_count = 0;
}

void push_game_history(uint64_t hash)
{
    game_history[move_count++] = hash;
}

void pop_game_history(uint64_t hash)
{
    move_count--;
}

bool threefold_repetition()
{
    uint64_t current = game_history[move_count - 1];
    int repetitions = 1;

    for (int i = move_count - 3; i >= 0; i -= 2)
    {
        if (game_history[i] == current)
        {
            repetitions++;
            if (repetitions >= 3)
                return true;
        }
    }

    return false;
}