#include "test_symetric_heuristic.h"
#include "unity.h"

#include "tests/test_utils.h"
#include "tests/test_positions.h"

#include "utils/fen.h"
#include "utils/bitboard.h"

#include "engine/piece_moves.h"

#include "algorithm/heuristic/heuristic.h"
#include "algorithm/heuristic/position_score.h"

#include <math.h>
#include <stdio.h>

void test_symmetry(const TestPosition *test_position)
{
    Board board = fen_to_board(test_position->fen);
    BoardState board_state = board_to_board_state(&board);

    double score = score_board(&board_state);
    if (board.side_to_move == BLACK)
        score = -score;

    Board flipped_board = flip_board(&board);
    BoardState flipped_board_state = board_to_board_state(&flipped_board);

    double flipped_score = score_board(&flipped_board_state);
    if (flipped_board.side_to_move == BLACK)
        flipped_score = -flipped_score;

    ASSERT_DOUBLE_EQUALS(score, -flipped_score, "Symmetry test failed for FEN: %s", test_position->fen);
}

void test_symmetry_wrapper(void)
{
    for (size_t i = 0; i < sizeof(test_positions) / sizeof(TestPosition); i++)
        test_symmetry(&test_positions[i]);
}
