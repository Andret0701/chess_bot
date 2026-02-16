#include "game_history.h"
#include <stdlib.h>
#include "algorithm/zobrist_hash.h"

#define MAX_GAME_HISTORY 20000

uint64_t game_history[MAX_GAME_HISTORY] = {0};
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

void force_push_game_history(Board *from, Board *to)
{
    uint64_t hash = hash_board(to);
    bool reset_occurred = has_50_move_reset_occurred(*from, *to);
    if (reset_occurred)
        reset_game_history();
    push_game_history(hash);
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
    if (move_count < 9)
        return false;

    uint64_t current = game_history[move_count - 1];
    int repetitions = 1;

    for (int i = move_count - 5; i >= 0; i -= 2)
    {
        if (game_history[i] == current)
        {
            repetitions++;
            if (repetitions >= 3)
                return true;
            i -= 2;
        }
    }

    return false;
}