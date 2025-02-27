#include "capture_move_test.h"
#include <stdbool.h>
#include <stdint.h>
#include "../../utils/board.h"
#include "../board_stack.h"
#include "../piece_moves.h"
#include "count_tests.h"
#include <stdio.h>
#include "../../utils/fen.h"

bool has_captured(Board from, Board to)
{
    uint64_t black_pieces_from = pieces_to_bitboard(&from.black_pieces);
    uint64_t white_pieces_from = pieces_to_bitboard(&from.white_pieces);
    uint64_t black_pieces_to = pieces_to_bitboard(&to.black_pieces);
    uint64_t white_pieces_to = pieces_to_bitboard(&to.white_pieces);

    if (from.side_to_move == WHITE)
        return (black_pieces_from & ~black_pieces_to) != 0;
    else
        return (white_pieces_from & ~white_pieces_to) != 0;
}

void every_move_is_capture_test()
{
    BoardStack *stack = create_board_stack(65535);
    uint16_t number_of_tests = sizeof(tests) / sizeof(Test);
    uint16_t passed = 0;
    uint16_t test_count = 0;

    for (uint16_t i = 0; i < number_of_tests; i++)
    {
        Board board = fen_to_board(tests[i].fen);
        BoardState board_state = board_to_board_state(&board);

        stack->count = 0;
        generate_captures(&board_state, stack);
        for (uint16_t j = 0; j < stack->count; j++)
        {
            if (!has_captured(board, stack->boards[j].board))
            {
                printf(":( Test %u failed. Expected every move to be a capture\n", test_count);
                print_board(&board);
                print_board(&stack->boards[j].board);
                return;
            }
            else
            {
                passed++;
            }
            test_count++;
        }

        board = flip_board(&board);
        board_state = board_to_board_state(&board);
        stack->count = 0;
        generate_captures(&board_state, stack);
        for (uint16_t j = 0; j < stack->count; j++)
        {
            if (!has_captured(board, stack->boards[j].board))
            {
                printf(":( Flipped test %u failed. Expected every move to be a capture\n", test_count);
                print_board(&board);
                print_board(&stack->boards[j].board);
                return;
            }
            else
            {
                passed++;
            }
            test_count++;
        }
    }

    printf("Passed %u out of %u tests\n", passed, test_count);
    destroy_board_stack(stack);
}

void every_capture_is_valid_test()
{
    BoardStack *stack = create_board_stack(65535);
    BoardStack *capture_stack = create_board_stack(65535);
    uint16_t number_of_tests = sizeof(tests) / sizeof(Test);
    uint16_t passed = 0;
    uint16_t test_count = 0;

    for (uint16_t i = 0; i < number_of_tests; i++)
    {
        Board board = fen_to_board(tests[i].fen);
        BoardState board_state = board_to_board_state(&board);

        stack->count = 0;
        generate_moves(&board_state, stack);
        capture_stack->count = 0;
        generate_captures(&board_state, capture_stack);

        for (uint16_t j = 0; j < capture_stack->count; j++)
        {
            bool found = false;
            for (uint16_t k = 0; k < stack->count; k++)
            {
                if (board_equals(&stack->boards[k].board, &capture_stack->boards[j].board))
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                printf(":( Test %u failed. Expected every capture move to be present in the move list\n", test_count);
                print_board(&board);
                print_board(&capture_stack->boards[j].board);
                return;
            }
            else
            {
                passed++;
            }
            test_count++;
        }

        board = flip_board(&board);
        board_state = board_to_board_state(&board);
        stack->count = 0;
        generate_moves(&board_state, stack);
        capture_stack->count = 0;
        generate_captures(&board_state, capture_stack);

        for (uint16_t j = 0; j < capture_stack->count; j++)
        {
            bool found = false;
            for (uint16_t k = 0; k < stack->count; k++)
            {
                if (board_equals(&stack->boards[k].board, &capture_stack->boards[j].board))
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                printf(":( Flipped test %u failed. Expected every capture move to be present in the move list\n", test_count);
                print_board(&board);
                print_board(&capture_stack->boards[j].board);
                return;
            }
            else
            {
                passed++;
            }
            test_count++;
        }
    }

    printf("Passed %u out of %u tests\n", passed, test_count);
    destroy_board_stack(stack);
}

void every_capture_is_generated_test()
{
    BoardStack *stack = create_board_stack(65535);
    BoardStack *capture_stack = create_board_stack(65535);
    uint16_t number_of_tests = sizeof(tests) / sizeof(Test);
    uint16_t passed = 0;
    uint16_t test_count = 0;

    for (uint16_t i = 0; i < number_of_tests; i++)
    {
        Board board = fen_to_board(tests[i].fen);
        BoardState board_state = board_to_board_state(&board);

        stack->count = 0;
        generate_moves(&board_state, stack);
        capture_stack->count = 0;
        generate_captures(&board_state, capture_stack);

        for (uint16_t j = 0; j < stack->count; j++)
        {
            if (!has_captured(board, stack->boards[j].board))
                continue;

            bool found = false;
            for (uint16_t k = 0; k < capture_stack->count; k++)
            {
                if (board_equals(&stack->boards[j].board, &capture_stack->boards[k].board))
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                printf(":( Test %u failed. Expected every capture to be generated\n", test_count);
                print_board(&board);
                print_board(&stack->boards[j].board);
                return;
            }
            else
            {
                passed++;
            }
            test_count++;
        }

        board = flip_board(&board);
        board_state = board_to_board_state(&board);
        stack->count = 0;
        generate_moves(&board_state, stack);
        capture_stack->count = 0;
        generate_captures(&board_state, capture_stack);

        for (uint16_t j = 0; j < stack->count; j++)
        {
            if (!has_captured(board, stack->boards[j].board))
                continue;

            bool found = false;
            for (uint16_t k = 0; k < capture_stack->count; k++)
            {
                if (board_equals(&stack->boards[j].board, &capture_stack->boards[k].board))
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                printf(":( Flipped test %u failed. Expected every capture to be generated\n", test_count);
                print_board(&board);
                print_board(&stack->boards[j].board);
                return;
            }
            else
            {
                passed++;
            }
            test_count++;
        }
    }

    printf("Passed %u out of %u tests\n", passed, test_count);
    destroy_board_stack(stack);
}

void run_capture_move_tests()
{
    printf("Running every capture move has to be capture test\n");
    every_move_is_capture_test();
    printf("Running every capture move has to be valid test\n");
    every_capture_is_valid_test();
    printf("Running every capture move has to be generated test\n");
    every_capture_is_generated_test();
}