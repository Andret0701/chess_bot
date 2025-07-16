#include "reductions.h"

uint8_t reductions[MAX_DEPTH][MAX_MOVES];
void init_reductions()
{
    for (uint8_t depth = 0; depth < MAX_DEPTH; depth++)
    {
        for (uint16_t move_number = 0; move_number < MAX_MOVES; move_number++)
        {
            reductions[depth][move_number] = (int)(1.35 + log(depth) * log(move_number) / 2.75);
        }
    }
}
uint8_t calculate_reduction(BoardState *current, BoardState *next, uint8_t depth, uint16_t move_number)
{
    __builtin_prefetch(&reductions[depth][move_number], 0, 3);

    bool is_capture = is_move_capture(current, next);
    bool is_promo = is_move_promotion(current, next);
    bool is_check = is_move_check(next);
    bool is_quiet = !is_capture && !is_promo && !is_check;

    bool do_reduction = is_quiet && (move_number >= 2) && (depth >= 2);
    if (do_reduction)
        return reductions[depth][move_number];

    return 0; // No reduction
}