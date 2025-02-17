#include "can_move_test.h"
#include "count_tests.h"
#include <stdio.h>
#include "../../utils/fen.h"
#include "../board_stack.h"
#include "../piece_moves.h"

void find_false_move(BoardState *board_state)
{
    for (uint8_t x = 0; x < 8; x++)
    {
        for (uint8_t y = 0; y < 8; y++)
        {
            if (board_state->board.side_to_move == WHITE)
            {
                if (board_state->white_pieces & position_to_bitboard(x, y))
                {
                    if (board_state->board.white_pieces.pawns & position_to_bitboard(x, y))
                    {
                        if (white_pawn_can_move(board_state, x, y))
                        {
                            printf("White pawn can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                    else if (board_state->board.white_pieces.knights & position_to_bitboard(x, y))
                    {
                        if (white_knight_can_move(board_state, x, y))
                        {
                            printf("White knight can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                    else if (board_state->board.white_pieces.bishops & position_to_bitboard(x, y))
                    {
                        if (white_bishop_can_move(board_state, x, y))
                        {
                            printf("White bishop can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                    else if (board_state->board.white_pieces.rooks & position_to_bitboard(x, y))
                    {
                        if (white_rook_can_move(board_state, x, y))
                        {
                            printf("White rook can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                    else if (board_state->board.white_pieces.queens & position_to_bitboard(x, y))
                    {
                        if (white_queen_can_move(board_state, x, y))
                        {
                            printf("White queen can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                    else
                    {
                        if (white_king_can_move(board_state, x, y))
                        {
                            printf("White king can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                }
            }
            else
            {
                if (board_state->black_pieces & position_to_bitboard(x, y))
                {
                    if (board_state->board.black_pieces.pawns & position_to_bitboard(x, y))
                    {
                        if (black_pawn_can_move(board_state, x, y))
                        {
                            printf("Black pawn can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                    else if (board_state->board.black_pieces.knights & position_to_bitboard(x, y))
                    {
                        if (black_knight_can_move(board_state, x, y))
                        {
                            printf("Black knight can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                    else if (board_state->board.black_pieces.bishops & position_to_bitboard(x, y))
                    {
                        if (black_bishop_can_move(board_state, x, y))
                        {
                            printf("Black bishop can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                    else if (board_state->board.black_pieces.rooks & position_to_bitboard(x, y))
                    {
                        if (black_rook_can_move(board_state, x, y))
                        {
                            printf("Black rook can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                    else if (board_state->board.black_pieces.queens & position_to_bitboard(x, y))
                    {
                        if (black_queen_can_move(board_state, x, y))
                        {
                            printf("Black queen can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                    else
                    {
                        if (black_king_can_move(board_state, x, y))
                        {
                            printf("Black king can move at %d, %d\n", x, y);
                            return;
                        }
                    }
                }
            }
        }
    }
}

bool recursive_can_move(BoardState *board_state, uint8_t depth, BoardStack *stack)
{
    if (depth == 0)
        return true;

    uint32_t base = stack->count;
    generate_moves(board_state, stack);

    bool finished = base == stack->count;
    bool finished2 = is_finished(board_state);

    if (finished != finished2)
    {
        printf("Finished mismatch\n");
        printf("Normal: %d\n", finished);
        printf("Finished: %d\n", finished2);
        printf("Possible moves: %d\n", stack->count - base);
        printf("FEN: %s\n", board_to_fen(&board_state->board));
        if (finished && !finished2)
        {
            find_false_move(board_state);
        }

        stack->count = base;
        return false;
    }

    if (finished)
        return true;

    for (uint16_t i = base; i < stack->count; i++)
    {
        if (!recursive_can_move(&stack->boards[i], depth - 1, stack))
        {
            stack->count = base;
            return false;
        }
    }

    stack->count = base;
    return true;
}

void run_can_move_tests()
{
    BoardStack *stack = create_board_stack(65535);
    uint16_t number_of_tests = sizeof(tests) / sizeof(Test);
    uint16_t passed_tests = 0;
    for (uint16_t i = 0; i < number_of_tests; i++)
    {
        stack->count = 0;
        Board board = fen_to_board(tests[i].fen);
        BoardState board_state = board_to_board_state(&board);

        uint8_t depth = tests[i].depth;
        if (depth < 3)
            depth = 3;

        bool passed = recursive_can_move(&board_state, depth, stack);
        if (!passed)
        {
            printf(":( Test %u failed\n", i);
        }
        else
        {
            // printf(":) Test %u passed\n", i);
            passed_tests++;
        }

        board = flip_board(&board);
        board_state = board_to_board_state(&board);
        passed = recursive_can_move(&board_state, depth, stack);
        if (!passed)
        {
            printf(":( Flipped %u failed\n", i);
        }
        else
        {
            // printf(":) Flipped %u passed\n", i);
            passed_tests++;
        }
    }

    printf("Passed %u out of %u tests\n", passed_tests, number_of_tests * 2);
    destroy_board_stack(stack);
}