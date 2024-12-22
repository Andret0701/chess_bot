#include "game_history.h"

#define MAX_GAME_HISTORY 1000

Board game_history[MAX_GAME_HISTORY];
uint16_t game_history_index = 0;

void reset_game_history()
{
    game_history_index = 0;
}

void push_game_history(Board board)
{
    game_history[game_history_index++] = board;
}

void pop_game_history()
{
    game_history_index--;
}

bool threefold_repetition()
{
    if (game_history_index < 4)  // Fewer than 4 positions mean no threefold repetition
        return false;

    Board *current = &game_history[game_history_index - 1];
    int repetitions = 1;

    // Iterate only through earlier positions
    for (int i = game_history_index - 2; i >= 0; i--)
    {
        if (board_equals(current, &game_history[i]))
        {
            repetitions++;
            if (repetitions >= 3)
                return true;
        }
    }

    return false;
}

