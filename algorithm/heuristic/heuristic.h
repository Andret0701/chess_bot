#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "utils/board.h"
#include "algorithm/board_score.h"
#include "engine/board_state.h"

#define MATE_SCORE (INT32_MAX - 10000)
#define MAX_DEPTH 255
#define INF (MATE_SCORE + MAX_DEPTH + 1)
#define BEST_SCORE (INF)
#define WORST_SCORE (-INF)

bool has_insufficient_material(Board *board);
int32_t score_board(BoardState *board_state);
Result get_result(BoardState *board_state, bool is_finished);
int32_t get_result_score(Result result, uint8_t depth);
bool is_winning(int32_t score);
bool is_losing(int32_t score);
bool is_mate_score(int32_t score);
int32_t value_to_tt(int32_t score, uint8_t depth);
int32_t value_from_tt(int32_t score, uint8_t depth);