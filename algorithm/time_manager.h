#pragma once
#include "../utils/board.h"

static const double MIN_MOVE_TIME = 0.05;  // sec – never play 0‑time moves
static const double MAX_TIME_RATIO = 0.30; // spend at most 30 % of the clock
static const double STEAL_RATIO = 0.15;    // borrow up to 15 % from future
static const double INC_USAGE = 0.80;      // use 80 % of the increment
static const int DEFAULT_MTG = 40;         // default moves‑to‑go estimate
double get_time_allocation(int wtime, int btime,
                           int winc, int binc,
                           Color side_to_move);