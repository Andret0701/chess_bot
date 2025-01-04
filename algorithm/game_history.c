#include "game_history.h"

#define MAX_GAME_HISTORY 10000

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

void print_game_history()
{
    printf("Game history:\n");
    for (int i = 0; i < game_history_index; i++)
    {
        print_board(&game_history[i]);
        printf("\n");
    }
    printf("---\n");
}

bool threefold_repetition()
{
    if (game_history_index < 6) // Fewer than 4 positions mean no threefold repetition
        return false;

    Board *current = &game_history[game_history_index - 1];
    int repetitions = 1;

    // Iterate through all earlier positions
    for (int i = game_history_index - 3; i >= 0; i -= 2)
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

bool has_repeated_position()
{
    if (game_history_index < 6) // Fewer than 4 positions mean no repeated position
        return false;

    Board *current = &game_history[game_history_index - 1];

    // Iterate only through earlier positions
    for (int i = game_history_index - 3; i >= 0; i -= 2)
    {
        if (board_equals(current, &game_history[i]))
            return true;
    }

    return false;
}
