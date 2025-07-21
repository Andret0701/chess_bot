#include "symetric_heuristic_test.h"

#include "../../../utils/fen.h"
#include "../../../engine/piece_moves.h"
#include "../../../utils/bitboard.h"
#include <math.h>
#include <stdio.h>
#include "../../../engine/tests/count_tests.h"
#include "../heuristic.h"

#define EPS 1e-7 /* tolerance for double comparison */

/* ------------------------------------------------------------------------- */
static bool run_single_eval_test(const Test *t)
{
    Board b = fen_to_board(t->fen);
    BoardState bs = board_to_board_state(&b);
    double orig_score = score_board(&bs);
    double orig_pos_score = get_position_score(&b, get_game_phase(&b));
    // flip orig_pos_score if black to move
    if (b.side_to_move == BLACK)
        orig_pos_score = -orig_pos_score;

    Board flipped = flip_board(&b);
    BoardState flipped_bs = board_to_board_state(&flipped);
    double flip_score = score_board(&flipped_bs);
    double flip_pos_score = get_position_score(&flipped, get_game_phase(&flipped));
    // flip flip_pos_score if black to move
    if (flipped.side_to_move == BLACK)
        flip_pos_score = -flip_pos_score;

    if (fabs(orig_pos_score - flip_pos_score) > EPS)
    {
        printf(":( Position score mismatch for FEN:\n    %s\n", t->fen);
        printf("    orig = %f  flipped = %f  delta = %.10f\n",
               orig_pos_score, flip_pos_score, fabs(orig_pos_score - flip_pos_score));
        return false;
    }

    if (fabs(orig_score - flip_score) > EPS)
    {
        printf(":( Heuristic mismatch for FEN:\n    %s\n", t->fen);
        printf("    orig = %f  flipped = %f  delta = %.10f\n", orig_score, flip_score, fabs(orig_score - flip_score));
        return false;
    }

    return true;
}

/* ------------------------------------------------------------------------- */
void run_heuristic_eval_tests()
{
    uint64_t total = 0;
    uint64_t passed = 0;

    for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i)
    {
        bool r = run_single_eval_test(&tests[i]);
        passed += r;
        total++;
    }

    if (passed == total)
        printf(":) Heuristic evaluation passed all %llu tests\n", total);
    else
        printf(":( Heuristic evaluation passed %llu / %llu tests\n",
               passed, total);
}
