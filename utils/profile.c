#include "profile.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "fen.h"
#include "move.h"
#include "engine/board_stack.h"
#include "engine/piece_moves.h"
#include "algorithm/bot.h"
#include "algorithm/game_history.h"
#include "algorithm/heuristic/heuristic.h"
#include "algorithm/heuristic/heuristic_values.h"
#include "algorithm/zobrist_hash.h"

void play_game(double time_seconds, double increment_seconds)
{
    Board board = fen_to_board(STARTFEN);
    int wtime = time_seconds * 1000;
    int btime = time_seconds * 1000;
    int winc = increment_seconds * 1000;
    int binc = increment_seconds * 1000;

    uint64_t hash = hash_board(&board);

    reset_game_history();
    push_game_history(hash);
    BoardState board_state = board_to_board_state(&board);
    while (true)
    {
        clock_t start = clock();
        BotResult result = run_time_bot(board, wtime, btime, winc, binc);
        clock_t end = clock();
        double time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        // take away the time from flags and add increment
        if (board.side_to_move == WHITE)
        {
            wtime -= time_used * 1000;
            if (wtime < 0)
            {
                printf("White ran out of time\n");
                break;
            }
            wtime += winc;
        }
        else
        {
            btime -= time_used * 1000;
            if (btime < 0)
            {
                printf("Black ran out of time\n");
                break;
            }
            btime += binc;
        }

        Color side_to_move = board.side_to_move;
        board = apply_move(&board, result.move);
        hash = hash_board(&board);
        if (board.side_to_move == side_to_move)
        {
            printf("Invalid move: %s\n", result.move);
            print_board(&board);
            exit(1);
        }

        push_game_history(hash);
        board_state = board_to_board_state(&board);
        print_board(&board);

        double score_value = ((double)result.score) / ((double)HEURISTIC_SCALE * 24);
        printf("Move: %s, Score: %.2f, Depth: %d\n", result.move, score_value, result.depth);
        printf("White time: %.1f, Black time: %.1f\n", wtime / 1000.0, btime / 1000.0);

        if (threefold_repetition())
        {
            printf("Draw by threefold repetition\n");
            break;
        }

        // if (has_50_move_rule_occurred())
        // {
        //     printf("Draw by 50 move rule\n");
        //     break;
        // }

        if (has_insufficient_material(&board))
        {
            printf("Draw by insufficient material\n");
            break;
        }

        Result game_result = get_game_result(&board_state);
        if (board_state.board.side_to_move == BLACK && game_result == LOST)
        {
            printf("White checkmated black\n");
            break;
        }
        else if (board_state.board.side_to_move == WHITE && game_result == LOST)
        {
            printf("Black checkmated white\n");
            break;
        }
        else if (game_result == STALEMATE)
        {
            printf("Stalemate\n");
            break;
        }
        else if (game_result == INSUFFICIENT_MATERIAL)
        {
            printf("Draw by insufficient material\n");
            break;
        }
        else if (game_result == FIFTY_MOVE_RULE)
        {
            printf("Draw by 50 move rule\n");
            break;
        }
        else if (game_result == THREEFOLD_REPETITION)
        {
            printf("Draw by threefold repetition\n");
            break;
        }

        // TT_log_stats();

        printf("\n");
    }

    print_move_statistics();
}