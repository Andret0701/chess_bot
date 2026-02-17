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
#include "time_manager.h"
#include "zobrist_hash.h"
#include "heuristic/heuristic.h"
#include "heuristic/heuristic_values.h"
#include <inttypes.h>
#include "algorithm/move_sort.h"

#define DEBUG_INFO false

#define MAX_MOVES 300

void print_bot_result(BotResult result)
{
    double score_value = ((double)result.score) / ((double)HEURISTIC_SCALE * 24);
    printf("Move: %s, Depth: %d (Score: %.2f, Depth: %d)\n",
           result.move,
           result.depth,
           score_value,
           result.depth);
}

// BoardScore move_scores[MAX_DEPTH][MAX_MOVES];
// void print_out_search_info(BoardStack *stack, Board *board, BoardState *best_board, BoardScore best_score, uint8_t depth, uint16_t cancelled_index, double seconds)
// {
//     FILE *file = fopen("search_info.txt", "a");
//     if (file == NULL)
//     {
//         perror("Failed to open file");
//         return;
//     }

//     if (cancelled_index == 0)
//     {
//         depth--;
//         cancelled_index = stack->count + 1;
//     }

//     fprintf(file, "Move: %d\n", get_full_move_count());
//     fprintf(file, "Fen: %s\n", board_to_fen(board));
//     if (best_board->white_check)
//         fprintf(file, "White is in check\n");
//     if (best_board->black_check)
//         fprintf(file, "Black is in check\n");
//     fprintf(file, "It is %s's turn\n", board->side_to_move == WHITE ? "White" : "Black");
//     fprintf(file, "Time: %.2f seconds\n", seconds);
//     fprintf(file, "The best move is %s with a score of %f, depth of %d, and result %s\n",
//             board_to_move(board, &best_board->board),
//             best_score.score,
//             best_score.depth,
//             result_to_string(best_score.result));

//     fprintf(file, "Move: | ");
//     for (int16_t d = depth; d >= 0; d--)
//     {
//         fprintf(file, "Depth: %-13d", d);
//         if (d != 0)
//             fprintf(file, " | ");
//     }
//     fprintf(file, "\n");

//     for (uint16_t i = 0; i < stack->count; i++)
//     {
//         fprintf(file, "%-5s | ", board_to_move(board, &stack->boards[i].board));
//         for (int16_t d = depth; d >= 0; d--)
//         {
//             if (i == cancelled_index && d == depth)
//                 fprintf(file, "---                 ");
//             else if (i >= cancelled_index && d == depth)
//                 fprintf(file, "                    ");
//             else
//             {
//                 if (move_scores[d][i].result == UNKNOWN)
//                     fprintf(file, "%-10.2f Depth: %-2d",
//                             move_scores[d][i].score,
//                             move_scores[d][i].depth);
//                 else
//                     fprintf(file, "%-10s Depth: %-2d",
//                             result_to_string(move_scores[d][i].result),
//                             move_scores[d][i].depth);
//             }
//             if (d != 0)
//                 fprintf(file, " | ");
//         }
//         fprintf(file, "\n");
//     }
//     fprintf(file, "\n");

//     fclose(file);
// }

// void updated_best_board(BoardState **best_board, BoardScore *best_score, BoardState *current_board_state, BoardScore score)
// {
//     if (is_greater_score(score, *best_score))
//     {
//         *best_board = current_board_state;
//         *best_score = score;
//     }
// }

BotMove moves[MAX_MOVES];
BotResult run_bot(Board board, bool use_max_time, double seconds, bool use_max_depth, uint8_t max_depth, bool use_max_nodes, uint64_t max_nodes)
{
    clock_t start = clock();
    uint64_t nodes_searched = 0;
    bool search_cancelled = false;
    TT_clear_generation();
    clear_killer_moves();

    BoardState board_state = board_to_board_state(&board);
    BoardStack *stack = create_board_stack(BOARD_STACK_SIZE);
    generate_moves(&board_state, stack);

    uint16_t num_moves = stack->count;
    for (uint16_t i = 0; i < num_moves; i++)
        moves[i].board = &stack->boards[i];

    for (uint8_t depth = 0;; depth++)
    {
        int32_t best_score = WORST_SCORE;
        BoardState *best_board = moves[0].board;
        for (uint16_t i = 0; i < num_moves; i++)
        {
            if (depth != 0 && is_losing(moves[i].score))
            {
                if (moves[i].score > best_score)
                {
                    best_board = moves[i].board;
                    best_score = moves[i].score;
                }
                continue;
            }

            BoardState *current_board_state = moves[i].board;
            int32_t score = -nega_scout(current_board_state, stack, depth, 0, WORST_SCORE, -best_score, use_max_time, start, seconds, use_max_nodes, &nodes_searched, max_nodes, true, &search_cancelled);
            if (search_cancelled)
            {
                if (i == 0)
                {
                    best_board = moves[0].board;
                    best_score = moves[0].score;
                }

                // if (DEBUG_INFO)
                //     print_out_search_info(stack, &board, best_board, best_score, depth, i, seconds);
                if (i == 0)
                    depth--;

                BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
                destroy_board_stack(stack);
                return result;
            }

            moves[i].score = score;
            if (score > best_score)
            {
                best_board = current_board_state;
                best_score = score;
            }

            // If the move is winning. Do not search deeper.
            if (is_winning(best_score) && depth > 0)
            {
                // if (DEBUG_INFO)
                //     print_out_search_info(stack, &board, best_board, best_score, depth, i + 1, seconds);

                TT_store(hash_board(&board_state.board), depth, value_to_tt(best_score, 0), EXACT, best_board->move);

                BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
                destroy_board_stack(stack);
                return result;
            }
        }

        TT_store(hash_board(&board_state.board), depth, value_to_tt(best_score, 0), EXACT, best_board->move);

        // Sort moves array using insertion sort based on the score at current depth
        for (uint16_t i = 1; i < num_moves; ++i)
        {
            BotMove key = moves[i];
            int j = i - 1;
            while (j >= 0 && key.score > moves[j].score)
            {
                moves[j + 1] = moves[j];
                --j;
            }
            moves[j + 1] = key;
        }

        // // if no move is unknown
        // bool all_moves_known = true;
        // for (uint16_t i = 0; i < num_moves; i++)
        // {
        //     if (moves[i].score.result == UNKNOWN)
        //     {
        //         all_moves_known = false;
        //         break;
        //     }
        // }

        // if (all_moves_known)
        // {
        //     // if (DEBUG_INFO)
        //     //     print_out_search_info(stack, &board, best_board, best_score, depth, stack->count + 1, seconds);

        //     BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
        //     destroy_board_stack(stack);
        //     return result;
        // }

        if (depth + 1 >= MAX_DEPTH || (use_max_depth && depth + 1 >= max_depth))
        {
            // if (DEBUG_INFO)
            //     print_out_search_info(stack, &board, best_board, best_score, depth, stack->count + 1, seconds);
            BotResult result = {board_to_move(&board, &best_board->board), best_score, depth};
            destroy_board_stack(stack);
            return result;
        }
    }
}

BotResult run_depth_bot(Board board, uint8_t depth)
{
    return run_bot(board, false, 0, true, depth, false, 0);
}

BotResult run_time_bot(Board board, int wtime, int btime, int winc, int binc)
{
    double seconds = get_time_allocation(wtime, btime, winc, binc, board.side_to_move);
    return run_bot(board, true, seconds, false, 0, false, 0);
}

BotResult run_movetime_bot(Board board, int movetime)
{
    return run_bot(board, true, movetime / 1000.0, false, 0, false, 0);
}

BotResult run_nodes_bot(Board board, uint64_t nodes)
{
    return run_bot(board, false, 0, false, 0, true, nodes);
}

BotResult run_heuristic_bot(Board board)
{
    BoardState board_state = board_to_board_state(&board);
    BoardStack *stack = create_board_stack(BOARD_STACK_SIZE);
    generate_moves(&board_state, stack);
    int32_t best_score = INT32_MIN;
    uint16_t best_move_index = 0;
    for (uint16_t i = 0; i < stack->count; i++)
    {
        BoardState *next_board_state = &stack->boards[i];
        int32_t score = score_board(next_board_state);
        if (score > best_score)
        {
            best_score = score;
            best_move_index = i;
        }
    }
    BotResult result = {board_to_move(&board, &stack->boards[best_move_index].board), best_score, 0};
    destroy_board_stack(stack);
    return result;
}