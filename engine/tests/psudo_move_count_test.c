#include "psudo_move_count_test.h"
#include "validate_board.h"
#include "../../utils/fen.h"
#include "../piece_moves.h"
#include "count_tests.h"
#include <stdio.h>

uint64_t psudo_count_recursive_test(BoardState *board_state, uint8_t depth, BoardStack *board_stack, MoveStack *move_stack)
{
    validate_board(&board_state->board);
    if (depth == 0)
        return 1;

    uint32_t board_base = board_stack->count;
    uint32_t move_base = move_stack->count;
    generate_psudo_legal_moves(board_state, move_stack);
    generate_moves(board_state, board_stack);

    uint64_t total = 0;

    uint16_t count_legal_moves = 0;
    for (uint16_t i = move_base; i < move_stack->count; i++)
    {
        BoardState new_board_state = do_move(board_state, move_stack->moves[i]);
        if (!is_legal_move(&new_board_state))
            continue;

        bool found = false;
        for (uint16_t j = board_base; j < board_stack->count; j++)
        {
            if (board_stack->boards[j].move != move_stack->moves[i])
                continue;

            found = true;

            // board state should be the same as the one we just generated
            if (board_stack->boards[j].white_pieces != new_board_state.white_pieces ||
                board_stack->boards[j].black_pieces != new_board_state.black_pieces ||
                board_stack->boards[j].board.en_passant != new_board_state.board.en_passant ||
                board_stack->boards[j].board.castling_rights != new_board_state.board.castling_rights ||
                board_stack->boards[j].board.side_to_move != new_board_state.board.side_to_move ||
                board_stack->boards[j].has_castled != new_board_state.has_castled ||
                board_stack->boards[j].move != new_board_state.move)
            {
                printf("Error: Board state mismatch after move %u.\n", move_stack->moves[i]);
                printf("Expected white pieces: %llu, got: %llu\n", new_board_state.white_pieces, board_stack->boards[j].white_pieces);
                printf("Expected black pieces: %llu, got: %llu\n", new_board_state.black_pieces, board_stack->boards[j].black_pieces);
            }
            break;
        }

        if (!found)
        {
            printf("Error: Move %u not found in board stack.\n", move_stack->moves[i]);
            print_move(move_stack->moves[i]);
            printf("Board state:\n");
            print_board(&new_board_state.board);
            printf("White pieces: %llu, Black pieces: %llu\n", new_board_state.white_pieces, new_board_state.black_pieces);
        }

        validate_castling(board_state, &new_board_state);
        total += psudo_count_recursive_test(&new_board_state, depth - 1, board_stack, move_stack);
        count_legal_moves++;
    }

    if (count_legal_moves != (board_stack->count - board_base))
        printf("Warning: Expected %u legal moves, but found %u.\n", board_stack->count - board_base, count_legal_moves);

    board_stack->count = board_base;
    move_stack->count = move_base;

    return total;
}

void run_psudo_move_count_tests()
{
    BoardStack *board_stack = create_board_stack(65535);
    MoveStack *move_stack = create_move_stack(65535);
    uint16_t number_of_tests = sizeof(tests) / sizeof(Test);
    uint16_t passed = 0;
    for (uint16_t i = 0; i < number_of_tests; i++)
    {
        board_stack->count = 0;
        move_stack->count = 0;

        Board board = fen_to_board(tests[i].fen);
        BoardState board_state = board_to_board_state(&board);

        uint64_t result = psudo_count_recursive_test(&board_state, tests[i].depth, board_stack, move_stack);
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
        result = psudo_count_recursive_test(&board_state, tests[i].depth, board_stack, move_stack);
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
    destroy_board_stack(board_stack);
    destroy_move_stack(move_stack);
}
