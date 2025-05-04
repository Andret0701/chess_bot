#include "encoded_move_test.h"
#include "validate_board.h"
#include "../../utils/fen.h"
#include "../piece_moves.h"
#include "count_tests.h"
#include "../encoded_move.h"
#include <stdio.h>

bool check_encoded_move_recursive(BoardState *board_state, uint8_t depth, BoardStack *stack)
{
    if (depth == 0)
        return 1;

    uint32_t base = stack->count;
    generate_moves(board_state, stack);

    if (stack->count == base)
        return 0;

    for (uint16_t i = base; i < stack->count; i++)
    {
        uint16_t move = board_to_encoded_move(&board_state->board, &stack->boards[i].board);
        if (!encoded_move_equals(move, stack->boards[i].move))
        {
            printf(":( Test %u failed: \n", i);
            printf("  Should have been: ");
            print_move(move);
            printf("  Found have been: ");
            print_move(stack->boards[i].move);

            return false;
        }

        bool passed = check_encoded_move_recursive(&stack->boards[i], depth - 1, stack);
        if (!passed)
            return false;
    }

    stack->count = base;
    return true;
}

void run_encoded_move_tests()
{
    BoardStack *stack = create_board_stack(65535);
    uint16_t number_of_tests = sizeof(tests) / sizeof(Test);
    uint16_t num_passed = 0;
    for (uint16_t i = 0; i < number_of_tests; i++)
    {
        stack->count = 0;
        Board board = fen_to_board(tests[i].fen);
        BoardState board_state = board_to_board_state(&board);

        bool passed = check_encoded_move_recursive(&board_state, tests[i].depth, stack);
        if (!passed)
        {
            printf(":( Test %u failed. ", i);
            printf("FEN: %s\n\n", tests[i].fen);
        }
        else
        {
            // printf(":) Test %u passed. Expected %u, got %u\n", i, board_to_encoded_move(&board_state, &board), tests[i].move);
            num_passed++;
        }

        board = flip_board(&board);
        board_state = board_to_board_state(&board);
        passed = check_encoded_move_recursive(&board_state, tests[i].depth, stack);
        if (!passed)
        {
            printf(":( Flipped %u failed. ", i);
            printf("FEN: %s\n\n", tests[i].fen);
        }
        else
        {
            // printf(":) Flipped %u passed. Expected %u, got %u\n", i, board_to_encoded_move(&board_state, &board), tests[i].move);
            num_passed++;
        }
    }

    printf("Passed %u out of %u tests\n", num_passed, number_of_tests * 2);
    destroy_board_stack(stack);
}
