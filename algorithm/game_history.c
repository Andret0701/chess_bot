#include "game_history.h"

#define MAX_GAME_HISTORY 10000

uint8_t moves_since_permanent_change[MAX_GAME_HISTORY];
Board board_history[MAX_GAME_HISTORY];
uint16_t move_count = 0;

bool has_50_move_reset_occurred(Board from, Board to)
{
    // Check for pawn moves or promotions (different positions or counts)
    if (from.white_pieces.pawns != to.white_pieces.pawns ||
        from.black_pieces.pawns != to.black_pieces.pawns)
        return true;

    // Check for captures by comparing piece counts for all types
    if (__builtin_popcountll(from.white_pieces.knights) != __builtin_popcountll(to.white_pieces.knights) ||
        __builtin_popcountll(from.white_pieces.bishops) != __builtin_popcountll(to.white_pieces.bishops) ||
        __builtin_popcountll(from.white_pieces.rooks) != __builtin_popcountll(to.white_pieces.rooks) ||
        __builtin_popcountll(from.white_pieces.queens) != __builtin_popcountll(to.white_pieces.queens) ||
        __builtin_popcountll(from.black_pieces.knights) != __builtin_popcountll(to.black_pieces.knights) ||
        __builtin_popcountll(from.black_pieces.bishops) != __builtin_popcountll(to.black_pieces.bishops) ||
        __builtin_popcountll(from.black_pieces.rooks) != __builtin_popcountll(to.black_pieces.rooks) ||
        __builtin_popcountll(from.black_pieces.queens) != __builtin_popcountll(to.black_pieces.queens))
        return true;

    return false;
}

void reset_game_history()
{
    move_count = 0;
}
void push_game_history(Board board)
{
    board_history[move_count] = board;
    if (move_count == 0)
        moves_since_permanent_change[0] = 0;
    else
    {
        if (has_50_move_reset_occurred(board_history[move_count - 1], board))
            moves_since_permanent_change[move_count] = 0;
        else
            moves_since_permanent_change[move_count] = moves_since_permanent_change[move_count - 1] + 1;
    }
    move_count++;
}

void pop_game_history()
{
    move_count--;
}

void print_game_history()
{
    for (int i = 0; i < move_count; i++)
    {
        print_board(&board_history[i]);
        printf("\n");
    }
    printf("---\n");
}

bool threefold_repetition()
{
    if (move_count < 6) // Not enough moves for threefold repetition.
        return false;

    int earliest_move = move_count - 1 - moves_since_permanent_change[move_count - 1];

    // Not enough history to even allow 3 repetitions
    if (move_count - earliest_move < 6)
        return false;

    Board *current = &board_history[move_count - 1];
    int repetitions = 1;

    for (int i = move_count - 3; i >= earliest_move; i -= 2)
    {
        if (board_equals(current, &board_history[i]))
        {
            repetitions++;
            if (repetitions >= 3)
                return true;
        }
    }

    return false;
}

bool has_50_move_rule_occurred()
{
    if (move_count < 100)
        return false;

    return moves_since_permanent_change[move_count - 1] >= 100;
}

uint8_t get_50_move_count()
{
    if (move_count <= 0)
        return 0;
    return moves_since_permanent_change[move_count - 1];
}

uint8_t get_full_move_count()
{
    return move_count / 2 + 1;
}

uint8_t get_half_move_count()
{
    return move_count;
}