#include "time_manager.h"

double get_time_allocation(int wtime, int btime,
                           int winc, int binc,
                           Color side_to_move)
{
    double remaining = ((side_to_move == WHITE) ? wtime : btime) / 1000.0;
    double inc = ((side_to_move == WHITE) ? winc : binc) / 1000.0;

    // (1) Estimate moves‑to‑go (mtg).
    int mtg = DEFAULT_MTG;
    if (remaining < 60)
        mtg = 25; // low clock – fewer moves left
    else if (remaining < 15)
        mtg = 12;

    // (2) Base allocation: even split + 80 % of increment.
    double alloc = remaining / mtg + inc * INC_USAGE;

    // (3) Allow borrowing time from future if we are far below the per‑move
    //     budget.  Cap the absolute use per move.
    double max_from_clock = remaining * MAX_TIME_RATIO;
    double steal_cap = remaining / mtg * STEAL_RATIO;
    if (alloc > max_from_clock)
        alloc = max_from_clock;
    else if (alloc < steal_cap)
        alloc += steal_cap;

    // (4) Absolute floor – never blitz completely.
    if (alloc < MIN_MOVE_TIME)
        alloc = MIN_MOVE_TIME;
    return alloc;
}