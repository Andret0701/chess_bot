#include "bot.h"
#include <stdio.h>
#include "../utils/board.h"
#include "../engine/board_stack.h"
#include "../utils/fen.h"
#include "min_max.h"
#include "../engine/piece_moves.h"
#include "../utils/move.h"
#include "game_history.h"

#define BOARD_STACK_SIZE 65535
#define MAX_DEPTH 100
#define MAX_MOVES 300

void print_bot_result(BotResult result)
{
    printf("Move: %s, Depth: %d (Score: %d, Depth: %d, Result: %s)\n",
           result.move,
           result.depth,
           result.score.score,
           result.score.depth,
           result.score.result == WHITE_WON ? "White won" : result.score.result == BLACK_WON ? "Black won"
                                                        : result.score.result == DRAW        ? "Draw"
                                                                                             : "Unknown");
}

BoardScore move_scores[MAX_DEPTH][MAX_MOVES];
void print_out_search_info(BoardStack *stack, Board *board, uint16_t best_index, uint8_t depth, uint16_t cancelled_index)
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
    if (stack->boards[best_index].white_check)
        fprintf(file, "White is in check\n");
    if (stack->boards[best_index].black_check)
        fprintf(file, "Black is in check\n");
    fprintf(file, "It is %s's turn\n", board->side_to_move == WHITE ? "White" : "Black");
    fprintf(file, "The best move is %s with a score of %d, depth of %d, and result %s\n",
            board_to_move(board, &stack->boards[best_index].board),
            move_scores[depth][best_index].score,
            move_scores[depth][best_index].depth,
            move_scores[depth][best_index].result == WHITE_WON ? "White won" : move_scores[depth][best_index].result == BLACK_WON ? "Black won"
                                                                           : move_scores[depth][best_index].result == DRAW        ? "Draw"
                                                                                                                                  : "Unknown");

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
                        move_scores[d][i].result == WHITE_WON ? "White won" : move_scores[d][i].result == BLACK_WON ? "Black won"
                                                                          : move_scores[d][i].result == DRAW        ? "Draw"
                                                                                                                    : "Unknown",
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

bool is_move_better(uint16_t i, uint16_t j, uint8_t depth, Color side_to_move)
{
    if (is_better_score(move_scores[depth][i], move_scores[depth][j], side_to_move))
        return true;

    if (depth > 0)
    {
        if (is_equal_score(move_scores[depth][i], move_scores[depth][j]))
            return is_move_better(i, j, depth - 1, side_to_move);
    }

    return false;
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

BotResult run_bot(BotFlags flags, Board board)
{
    clock_t start = clock();
    double seconds = get_time_allocation(flags, board.side_to_move);
    BoardState board_state = board_to_board_state(&board);

    BoardStack *stack = create_board_stack(BOARD_STACK_SIZE);
    generate_moves(&board_state, stack);

    uint8_t depth = 0;
    while (true)
    {
        BoardScore alpha = get_worst_score(WHITE);
        BoardScore beta = get_worst_score(BLACK);
        uint16_t best_index = 0;
        for (uint16_t i = 0; i < stack->count; i++)
        {
            if (depth != 0 && has_lost(move_scores[depth - 1][i].result, board.side_to_move))
            {
                move_scores[depth][i] = move_scores[depth - 1][i];
                continue;
            }

            BoardState *current_board_state = &stack->boards[i];
            SearchResult search_result = min_max(current_board_state, stack, depth, 0, alpha, beta, start, seconds);
            if (!search_result.valid)
            {

                print_out_search_info(stack, &board, best_index, depth, i);

                if (i == 0)
                    depth--;

                BotResult result = {board_to_move(&board, &stack->boards[best_index].board), move_scores[depth][best_index], depth};
                destroy_board_stack(stack);
                return result;
            }

            BoardScore score = search_result.board_score;
            move_scores[depth][i] = score;

            if (i == 0 || is_move_better(i, best_index, depth, board.side_to_move))
            {
                best_index = i;
                if (board.side_to_move == WHITE)
                    alpha = max_score(alpha, score, board.side_to_move);
                else
                    beta = max_score(beta, score, board.side_to_move);
            }
        }

        BoardScore best_score = move_scores[depth][best_index];
        if (has_won(best_score.result, board.side_to_move) && best_score.depth <= depth)
        {
            print_out_search_info(stack, &board, best_index, depth, stack->count + 1);

            BotResult result = {board_to_move(&board, &stack->boards[best_index].board), best_score, depth};
            destroy_board_stack(stack);
            return result;
        }

        // Sort the stack by score
        for (uint16_t i = 0; i < stack->count; i++)
        {
            for (uint16_t j = i + 1; j < stack->count; j++)
            {
                if (is_move_better(j, i, depth, board.side_to_move))
                {
                    BoardState temp = stack->boards[i];
                    stack->boards[i] = stack->boards[j];
                    stack->boards[j] = temp;

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
            print_out_search_info(stack, &board, best_index, depth, stack->count + 1);

            BotResult result = {board_to_move(&board, &stack->boards[best_index].board), move_scores[depth][best_index], depth};
            destroy_board_stack(stack);
            return result;
        }

        depth++;
    }
}