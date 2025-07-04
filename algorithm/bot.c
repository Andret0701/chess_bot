#include "bot.h"
#include <stdio.h>
#include "../utils/board.h"
#include "../engine/board_stack.h"
#include "../utils/fen.h"
#include "nega_scout.h"
#include "../engine/piece_moves.h"
#include "../utils/move.h"
#include "game_history.h"
#include "transposition_table.h"

#define DEBUG_INFO false

#define MAX_DEPTH 150
#define MAX_MOVES 300

void print_bot_result(BotResult result)
{
    printf("Move: %s, Depth: %d (Score: %.2f, Depth: %d, Result: %s)\n",
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
    fprintf(file, "The best move is %s with a score of %f, depth of %d, and result %s\n",
            board_to_move(board, &best_board->board),
            best_score.score,
            best_score.depth,
            best_score.result == WON ? "WON" : best_score.result == LOST ? "LOST"
                                           : best_score.result == DRAW   ? "DRAW"
                                                                         : "UNKNOWN");

    fprintf(file, "Move: | ");
    for (int16_t d = depth; d >= 0; d--)
    {
        fprintf(file, "Depth: %-13d", d);
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
                fprintf(file, "---                 ");
            else if (i >= cancelled_index && d == depth)
                fprintf(file, "                    ");
            else
            {
                if (move_scores[d][i].result == UNKNOWN)
                    fprintf(file, "%-10.2f Depth: %-2d",
                            move_scores[d][i].score,
                            move_scores[d][i].depth);
                else
                    fprintf(file, "%-10s Depth: %-2d",
                            move_scores[d][i].result == WON ? "WON" : move_scores[d][i].result == LOST ? "LOST"
                                                                                                       : "DRAW",
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

double get_time_allocation(int wtime, int btime, int winc, int binc, Color side_to_move)
{
    // Base minimal time allocation (in seconds) to avoid zero-time moves
    double base_time = 0.05; // Adjust as needed

    // Choose the appropriate values based on side to move
    double remaining_time = (side_to_move == WHITE) ? wtime : btime;
    double increment = (side_to_move == WHITE) ? winc : binc;

    // Convert remaining time and increment from milliseconds to seconds
    remaining_time /= 1000.0;
    increment /= 1000.0;

    // Calculate an initial allocation:
    // Divide remaining time evenly among moves left, then add half of the increment
    double time_per_move = remaining_time / 40.0; // Assuming 40 moves left
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

BotResult run_bot(Board board, bool use_max_time, double seconds, bool use_max_depth, uint8_t max_depth)
{
    clock_t start = clock();
    TT_clear_generation();
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
            SearchResult search_result = nega_scout(current_board_state, stack, depth, 0, WORST_SCORE, invert_score(best_score), use_max_time, start, seconds, true);
            search_result.board_score = invert_score(search_result.board_score);
            if (search_result.valid == INVALID)
            {
                if (best_board == NULL)
                {
                    best_board = &stack->boards[0];
                    if (depth != 0)
                        best_score = move_scores[depth - 1][0];
                }

                if (DEBUG_INFO)
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
                if (DEBUG_INFO)
                    print_out_search_info(stack, &board, best_board, best_score, depth, i + 1, seconds);
                BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
                destroy_board_stack(stack);
                return result;
            }
        }
        //   TT_store(hash_board(&board_state.board), board_state.board, depth, best_score.score, best_score.result, EXACT, best_board->move);

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
            if (DEBUG_INFO)
                print_out_search_info(stack, &board, best_board, best_score, depth, stack->count + 1, seconds);

            BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
            destroy_board_stack(stack);
            return result;
        }

        depth++;
        if (depth == MAX_DEPTH || (use_max_depth && depth == max_depth))
        {
            depth--;
            if (DEBUG_INFO)
                print_out_search_info(stack, &board, best_board, best_score, depth, stack->count + 1, seconds);
            BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
            destroy_board_stack(stack);
            return result;
        }
    }
}

BotResult run_depth_bot(Board board, uint8_t depth)
{
    return run_bot(board, false, 0, true, depth);
}

BotResult run_time_bot(Board board, int wtime, int btime, int winc, int binc)
{
    double seconds = get_time_allocation(wtime, btime, winc, binc, board.side_to_move);
    return run_bot(board, true, seconds, false, 0);
}

BotResult run_movetime_bot(Board board, int movetime)
{
    return run_bot(board, true, movetime / 1000.0, false, 0);
}