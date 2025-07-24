#include "attack_test.h"
#include "validate_board.h"
#include "../../utils/fen.h"
#include "../piece_moves.h"
#include "count_tests.h"
#include "../encoded_move.h"
#include <stdio.h>
#include "test_utils.h"

TestResults check_attack_recursive(BoardState *board_state, uint8_t depth, BoardStack *stack)
{
    uint32_t base = stack->count;
    generate_moves(board_state, stack);

    if (stack->count == base)
        return (TestResults){0, 0};

    TestResults results = {0, 0};
    for (uint16_t i = base; i < stack->count; i++)
    {
        Pieces white_attacks = {
            .pawns = generate_white_pawn_attacks(stack->boards[i].board.white_pieces.pawns),
            .knights = generate_knight_attacks(stack->boards[i].board.white_pieces.knights),
            .bishops = generate_bishop_attacks(stack->boards[i].occupied, stack->boards[i].board.white_pieces.bishops),
            .rooks = generate_rook_attacks(stack->boards[i].occupied, stack->boards[i].board.white_pieces.rooks),
            .queens = generate_queen_attacks(stack->boards[i].occupied, stack->boards[i].board.white_pieces.queens),
            .king = generate_king_attacks(stack->boards[i].board.white_pieces.king)};

        Pieces black_attacks = {
            .pawns = generate_black_pawn_attacks(stack->boards[i].board.black_pieces.pawns),
            .knights = generate_knight_attacks(stack->boards[i].board.black_pieces.knights),
            .bishops = generate_bishop_attacks(stack->boards[i].occupied, stack->boards[i].board.black_pieces.bishops),
            .rooks = generate_rook_attacks(stack->boards[i].occupied, stack->boards[i].board.black_pieces.rooks),
            .queens = generate_queen_attacks(stack->boards[i].occupied, stack->boards[i].board.black_pieces.queens),
            .king = generate_king_attacks(stack->boards[i].board.black_pieces.king)};

        if (pieces_equals(&white_attacks, &stack->boards[i].white_attacks) &&
            pieces_equals(&black_attacks, &stack->boards[i].black_attacks))
            results.passed++;
        else
        {
            printf(":( Test %u failed: \n", i);
        }

        results.count++;

        if (depth == 1)
            continue;

        TestResults sub_results = check_attack_recursive(&stack->boards[i], depth - 1, stack);
        results.passed += sub_results.passed;
        results.count += sub_results.count;
    }

    stack->count = base;
    return results;
}

void run_attack_tests()
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
        depth = 2;

        TestResults sub_results = check_attack_recursive(&board_state, depth, stack);
        results.passed += sub_results.passed;
        results.count += sub_results.count;

        board = flip_board(&board);
        board_state = board_to_board_state(&board);
        sub_results = check_attack_recursive(&board_state, depth, stack);
        results.passed += sub_results.passed;
        results.count += sub_results.count;
    }

    if (results.count != results.passed)
        printf(":( Passed %llu out of %llu tests\n", results.passed, results.count);
    else
        printf(":) Passed all %llu out of %llu tests\n", results.passed, results.count);
    destroy_board_stack(stack);
}
