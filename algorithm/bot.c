#include "bot.h"
#include <stdio.h>
#include "../utils/board.h"
#include "../engine/board_stack.h"
#include "../utils/fen.h"
#include "negamax.h"
#include "../engine/piece_moves.h"
#include "../utils/move.h"
#include "game_history.h"
#include "transposition_table.h"

#define BOARD_STACK_SIZE 65535
#define MAX_DEPTH 150
#define MAX_MOVES 300

void print_bot_result(BotResult result)
{
    printf("Move: %s, Depth: %d (Score: %d, Depth: %d, Result: %s)\n",
           result.move,
           result.depth,
           result.score.score,
           result.score.depth,
           result.score.result == WON ? "WON" : (result.score.result == LOST ? "LOST" : (result.score.result == DRAW ? "DRAW" : "UNKNOWN")));
}

BoardScore move_scores[MAX_DEPTH][MAX_MOVES];
void print_out_search_info(BoardStack *stack, Board *board, BoardState *best_board, BoardScore best_score, uint8_t depth, uint16_t cancelled_index, double seconds)
{
    FILE *file = fopen("search_info.txt", "a");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    if (cancelled_index == 0)
    {
        depth--;
        cancelled_index = stack->count + 1;
    }

    fprintf(file, "Move: %d\n", get_full_move_count());
    fprintf(file, "Fen: %s\n", board_to_fen(board));
    if (best_board->white_check)
        fprintf(file, "White is in check\n");
    if (best_board->black_check)
        fprintf(file, "Black is in check\n");
    fprintf(file, "It is %s's turn\n", board->side_to_move == WHITE ? "White" : "Black");
    fprintf(file, "Time: %.2f seconds\n", seconds);
    fprintf(file, "The best move is %s with a score of %d, depth of %d, and result %s\n",
            board_to_move(board, &best_board->board),
            best_score.score,
            best_score.depth,
            best_score.result == WON ? "WON" : best_score.result == LOST ? "LOST"
                                           : best_score.result == DRAW   ? "DRAW"
                                                                         : "UNKNOWN");

    fprintf(file, "Move: | ");
    for (int16_t d = depth; d >= 0; d--)
    {
        fprintf(file, "Depth: %-19d", d);
        if (d != 0)
            fprintf(file, " | ");
    }
    fprintf(file, "\n");

    for (uint16_t i = 0; i < stack->count; i++)
    {
        fprintf(file, "%-5s | ", board_to_move(board, &stack->boards[i].board));
        for (int16_t d = depth; d >= 0; d--)
        {
            if (i == cancelled_index && d == depth)
                fprintf(file, "---                       ");
            else if (i >= cancelled_index && d == depth)
                fprintf(file, "                          ");
            else
            {
                fprintf(file, "%-5d %-9s Depth: %-3d",
                        move_scores[d][i].score,
                        move_scores[d][i].result == WON ? "WON" : move_scores[d][i].result == LOST ? "LOST"
                                                              : move_scores[d][i].result == DRAW   ? "DRAW"
                                                                                                   : "UNKNOWN",
                        move_scores[d][i].depth);
            }
            if (d != 0)
                fprintf(file, " | ");
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");

    fclose(file);
}

double get_time_allocation(BotFlags flags, Color side_to_move)
{
    // If movestogo is 0, assume a default number of moves (e.g., 40 moves remaining in sudden death)
    if (flags.movestogo <= 0)
        flags.movestogo = 40;

    // Base minimal time allocation (in seconds) to avoid zero-time moves
    double base_time = 0.05; // Adjust as needed

    // Choose the appropriate values based on side to move
    double remaining_time = (side_to_move == WHITE) ? flags.wtime : flags.btime;
    double increment = (side_to_move == WHITE) ? flags.winc : flags.binc;

    // Convert remaining time and increment from milliseconds to seconds
    remaining_time /= 1000.0;
    increment /= 1000.0;

    // Calculate an initial allocation:
    // Divide remaining time evenly among moves left, then add half of the increment
    double time_per_move = remaining_time / flags.movestogo;
    double allocated_time = time_per_move + (increment * 0.7);

    // Avoid spending too much on one move:
    // For example, cap the allocation to 20% of the total remaining time.
    double max_allocation = remaining_time * 0.2;
    if (allocated_time > max_allocation)
        allocated_time = max_allocation;

    // Ensure a minimum time usage even if remaining time is very low
    if (allocated_time < base_time)
        allocated_time = base_time;

    return allocated_time;
}

void updated_best_board(BoardState **best_board, BoardScore *best_score, BoardState *current_board_state, BoardScore score)
{
    if (is_greater_score(score, *best_score))
    {
        *best_board = current_board_state;
        *best_score = score;
    }
}

BotResult run_bot(BotFlags flags, Board board)
{
    TT_clear_generation();
    clock_t start = clock();
    double seconds = get_time_allocation(flags, board.side_to_move);
    BoardState board_state = board_to_board_state(&board);

    BoardStack *stack = create_board_stack(BOARD_STACK_SIZE);
    generate_moves(&board_state, stack);

    uint8_t depth = 0;
    while (true)
    {
        BoardScore best_score = WORST_SCORE;
        BoardState *best_board = NULL;
        for (uint16_t i = 0; i < stack->count; i++)
        {
            if (depth != 0 && move_scores[depth - 1][i].result == LOST)
            {
                move_scores[depth][i] = move_scores[depth - 1][i];
                updated_best_board(&best_board, &best_score, &stack->boards[i], move_scores[depth][i]);
                continue;
            }

            BoardState *current_board_state = &stack->boards[i];
            SearchResult search_result = negamax(current_board_state, stack, depth, 0, WORST_SCORE, invert_score(best_score), start, seconds);
            search_result.board_score = invert_score(search_result.board_score);
            if (search_result.valid == INVALID)
            {
                if (best_board == NULL)
                {
                    best_board = &stack->boards[0];
                    if (depth != 0)
                        best_score = move_scores[depth - 1][0];
                }

                print_out_search_info(stack, &board, best_board, best_score, depth, i, seconds);
                if (i == 0)
                    depth--;

                BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
                destroy_board_stack(stack);
                return result;
            }

            BoardScore score = search_result.board_score;
            move_scores[depth][i] = score;
            updated_best_board(&best_board, &best_score, current_board_state, score);

            // If the move is winning. Do not search deeper.
            if (best_score.result == WON && best_score.depth <= depth)
            {
                print_out_search_info(stack, &board, best_board, best_score, depth, i + 1, seconds);
                BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
                destroy_board_stack(stack);
                return result;
            }
        }
        TT_store(hash_board(&board_state.board), depth, best_score.score, best_score.result, EXACT, best_board->move);

        // Sort the stack by score
        for (uint16_t i = 0; i < stack->count; i++)
        {
            for (uint16_t j = i + 1; j < stack->count; j++)
            {
                BoardScore a = move_scores[depth][i];
                BoardScore b = move_scores[depth][j];
                if (is_greater_score(b, a))
                {
                    // Swap the boards
                    BoardState temp_board = stack->boards[i];
                    stack->boards[i] = stack->boards[j];
                    stack->boards[j] = temp_board;

                    // Swap the scores
                    for (uint8_t d = 0; d <= depth; d++)
                    {
                        BoardScore temp_score = move_scores[d][i];
                        move_scores[d][i] = move_scores[d][j];
                        move_scores[d][j] = temp_score;
                    }
                }
            }
        }
        // // if only one move is not lost
        // uint16_t num_not_lost = 0;
        // uint16_t last_not_lost = 0;
        // for (uint16_t i = 0; i < stack->count; i++)
        // {
        //     if (!has_lost(move_scores[depth][i].result, board.side_to_move))
        //     {
        //         num_not_lost++;
        //         last_not_lost = i;
        //     }
        // }
        // if (num_not_lost == 1)
        // {
        //     print_out_search_info(stack, &board, last_not_lost, depth, stack->count + 1, nodes_searched, seconds);

        //     BotResult result = {board_to_move(&board, &stack->boards[last_not_lost].board), move_scores[depth][last_not_lost], depth};
        //     destroy_board_stack(stack);
        //     return result;
        // }

        // if no move is unknown
        bool all_moves_known = true;
        for (uint16_t i = 0; i < stack->count; i++)
        {
            if (move_scores[depth][i].result == UNKNOWN)
            {
                all_moves_known = false;
                break;
            }
        }

        if (all_moves_known)
        {
            print_out_search_info(stack, &board, best_board, best_score, depth, stack->count + 1, seconds);

            BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
            destroy_board_stack(stack);
            return result;
        }

        depth++;
        if (depth == MAX_DEPTH)
        {
            print_out_search_info(stack, &board, best_board, best_score, depth, stack->count + 1, seconds);
            BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
            destroy_board_stack(stack);
            return result;
        }
    }
}