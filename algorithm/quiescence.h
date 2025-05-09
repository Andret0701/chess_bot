#pragma once

#include <stdint.h>
#include <time.h>
#include "../utils/board.h"
#include "../engine/board_stack.h"
#include "board_score.h"
#include "search_result.h"

QuiescenceResult quiescence(BoardState *board_state,
                            BoardStack *stack,
                            double alpha,
                            double beta,
                            uint8_t depth,
                            clock_t start,
                            double seconds);
