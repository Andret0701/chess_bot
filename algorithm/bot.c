#include "bot.h"
#include <stdio.h>
#include "../board.h"
#include "../engine/board_stack.h"
#include "../fen.h"
#include "min_max.h"
#include "../engine/piece_moves.h"
#include "../move.h"

#define BOARD_STACK_SIZE 65535

void print_out_search_info(BoardStack *stack, Board *board, BoardScore *scores, uint16_t best_index, uint8_t depth, uint16_t cancelled_index)
{
    FILE *file = fopen("search_info.txt", "a");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "It is %s's turn\n", board->side_to_move == WHITE ? "White" : "Black");
    fprintf(file, "The best move is %s with a score of %d\n", board_to_move(board, &stack->boards[best_index].board), scores[best_index].score);
    fprintf(file, "Depth %d\n", depth);
    for (uint16_t i = 0; i < stack->count; i++)
    {
        if (i == cancelled_index)
            fprintf(file, "---\n");
        fprintf(file, "%s %d %s Depth: %d\n", board_to_move(board, &stack->boards[i].board), scores[i].score, scores[i].result == WHITE_WON ? "White won" : scores[i].result == BLACK_WON ? "Black won"
                                                                                                                                                        : scores[i].result == DRAW        ? "Draw"
                                                                                                                                                                                          : "Unknown",
                depth);
    }
    fprintf(file, "\n");

    fclose(file);
}

BotResult run_bot(char *fen, double seconds)
{
    clock_t start = clock();

    Board board = fen_to_board(fen);
    BoardState board_state = board_to_board_state(&board);

    BoardStack *stack = create_board_stack(BOARD_STACK_SIZE);
    BoardScore scores[BOARD_STACK_SIZE];
    for (uint16_t i = 0; i < BOARD_STACK_SIZE; i++)
        scores[i] = get_worst_score(board.side_to_move);

    generate_moves(&board_state, stack);

    uint8_t depth = 0;
    while (true)
    {
        BoardScore alpha = get_worst_score(WHITE);
        BoardScore beta = get_worst_score(BLACK);
        uint16_t best_index = 0;
        BoardScore best_score = get_worst_score(board.side_to_move);
        for (uint16_t i = 0; i < stack->count; i++)
        {
            BoardState *current_board_state = &stack->boards[i];
            SearchResult search_result = min_max(current_board_state, stack, depth, 0, alpha, beta, start, seconds);
            if (!search_result.valid)
            {

                print_out_search_info(stack, &board, scores, best_index, depth, i);

                if (i == 0)
                    depth--;
                BotResult result = {board_to_move(&board, &stack->boards[best_index].board), scores[best_index], depth};
                destroy_board_stack(stack);
                return result;
            }

            BoardScore score = search_result.board_score;
            scores[i] = score;

            if (is_better_score(score, best_score, board.side_to_move))
            {
                best_index = i;
                best_score = score;
                if (board.side_to_move == WHITE)
                    alpha = max_score(alpha, score, board.side_to_move);
                else
                    beta = max_score(beta, score, board.side_to_move);
            }
        }

        if (has_won(best_score.result, board.side_to_move))
        {
            print_out_search_info(stack, &board, scores, best_index, depth, -1);

            BotResult result = {board_to_move(&board, &stack->boards[best_index].board), scores[best_index], depth};
            destroy_board_stack(stack);
            return result;
        }

        // Sort the stack by score
        for (uint16_t i = 0; i < stack->count; i++)
        {
            for (uint16_t j = i + 1; j < stack->count; j++)
            {
                if (is_better_score(scores[j], scores[i], board.side_to_move))
                {
                    BoardState temp = stack->boards[i];
                    stack->boards[i] = stack->boards[j];
                    stack->boards[j] = temp;

                    BoardScore temp_score = scores[i];
                    scores[i] = scores[j];
                    scores[j] = temp_score;
                }
            }
        }
        depth++;
    }
}