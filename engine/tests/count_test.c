#include "count_test.h"
#include "validate_board.h"
#include "../../utils/fen.h"
#include "../piece_moves.h"
#include "count_tests.h"
#include <stdio.h>

uint64_t count_recursive_test(BoardState *board_state, uint8_t depth, BoardStack *stack)
{
    validate_board(&board_state->board);
    if (depth == 0)
        return 1;

    uint32_t base = stack->count;
    generate_moves(board_state, stack);

    if (stack->count == base)
        return 0;

    uint64_t total = 0;
    for (uint16_t i = base; i < stack->count; i++)
    {
        validate_castling(board_state, &stack->boards[i]);
        total += count_recursive_test(&stack->boards[i], depth - 1, stack);
    }

    stack->count = base;
    return total;
}

void run_count_tests()
{
    BoardStack *stack = create_board_stack(65535);
    uint16_t number_of_tests = sizeof(tests) / sizeof(Test);
    uint16_t passed = 0;
    for (uint16_t i = 0; i < number_of_tests; i++)
    {
        stack->count = 0;
        Board board = fen_to_board(tests[i].fen);
        BoardState board_state = board_to_board_state(&board);

        uint64_t result = count_recursive_test(&board_state, tests[i].depth, stack);
        if (result != tests[i].expected)
        {
            printf(":( Test %u failed. Expected %llu, got %llu. Off by %lld:\n", i, tests[i].expected, result, ((int64_t)result) - ((int64_t)tests[i].expected));
            printf("FEN: %s\n\n", tests[i].fen);
        }
        else
        {
            // printf(":) Test %u passed. Expected %llu, got %llu\n", i, tests[i].expected, result);
            passed++;
        }

        board = flip_board(&board);
        board_state = board_to_board_state(&board);
        result = count_recursive_test(&board_state, tests[i].depth, stack);
        if (result != tests[i].expected)
        {
            printf(":( Flipped %u failed. Expected %llu, got %llu. Off by %lld\n", i, tests[i].expected, result, ((int64_t)result) - ((int64_t)tests[i].expected));
            printf("FEN: %s\n\n", tests[i].fen);
        }
        else
        {
            // printf(":) Flipped %u passed. Expected %llu, got %llu\n", i, tests[i].expected, result);
            passed++;
        }
    }

    printf("Passed %u out of %u tests\n", passed, number_of_tests * 2);
    destroy_board_stack(stack);
}
