#include "encoded_move_test.h"
#include "validate_board.h"
#include "../../utils/fen.h"
#include "../piece_moves.h"
#include "count_tests.h"
#include "../encoded_move.h"
#include <stdio.h>

typedef struct
{
    uint64_t passed;
    uint64_t count;
} TestResults;

TestResults check_encoded_move_recursive(BoardState *board_state, uint8_t depth, BoardStack *stack)
{
    uint32_t base = stack->count;
    generate_moves(board_state, stack);

    if (stack->count == base)
        return (TestResults){0, 0};

    TestResults results = {0, 0};
    for (uint16_t i = base; i < stack->count; i++)
    {
        uint16_t move = board_to_encoded_move(&board_state->board, &stack->boards[i].board);
        if (encoded_move_equals(move, stack->boards[i].move))
            results.passed++;
        else
        {
            printf(":( Test %u failed: \n", i);
            printf("  Should have been: ");
            print_move(move);
            printf("  Found:            ");
            print_move(stack->boards[i].move);
        }

        results.count++;

        if (depth == 1)
            continue;

        TestResults sub_results = check_encoded_move_recursive(&stack->boards[i], depth - 1, stack);
        results.passed += sub_results.passed;
        results.count += sub_results.count;
    }

    stack->count = base;
    return results;
}

void run_encoded_move_tests()
{
    BoardStack *stack = create_board_stack(65535);
    uint16_t number_of_tests = sizeof(tests) / sizeof(Test);

    TestResults results = {0, 0};
    for (uint16_t i = 0; i < number_of_tests; i++)
    {
        stack->count = 0;
        Board board = fen_to_board(tests[i].fen);
        BoardState board_state = board_to_board_state(&board);

        uint8_t depth = tests[i].depth;
        if (depth < 3)
            depth = 3;

        TestResults sub_results = check_encoded_move_recursive(&board_state, depth, stack);
        results.passed += sub_results.passed;
        results.count += sub_results.count;

        board = flip_board(&board);
        board_state = board_to_board_state(&board);
        sub_results = check_encoded_move_recursive(&board_state, depth, stack);
        results.passed += sub_results.passed;
        results.count += sub_results.count;
    }

    if (results.count != results.passed)
        printf(":( Passed %llu out of %llu tests\n", results.passed, results.count);
    else
        printf(":) Passed all %llu out of %llu tests\n", results.passed, results.count);
    destroy_board_stack(stack);
}
