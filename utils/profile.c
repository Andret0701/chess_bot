#include "profile.h"

#include "../engine/board_stack.h"
#include "../algorithm/bot.h"
#include "fen.h"
#include <time.h>
#include <stdio.h>
#include "move.h"
#include "../algorithm/game_history.h"
#include "../algorithm/heuristic/heuristic.h"

void play_game(double time_seconds, double increment_seconds)
{
    Board board = fen_to_board(STARTFEN);
    BotFlags flags = {time_seconds * 1000, time_seconds * 1000, increment_seconds * 1000, increment_seconds * 1000, -1};

    reset_game_history();
    push_game_history(board);
    BoardState board_state = board_to_board_state(&board);
    while (true)
    {
        clock_t start = clock();
        BotResult result = run_bot(flags, board);
        clock_t end = clock();
        double time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        // take away the time from flags and add increment
        if (board.side_to_move == WHITE)
        {
            flags.wtime -= time_used * 1000;
            if (flags.wtime < 0)
            {
                printf("White ran out of time\n");
                break;
            }
            flags.wtime += flags.winc;
        }
        else
        {
            flags.btime -= time_used * 1000;
            if (flags.btime < 0)
            {
                printf("Black ran out of time\n");
                break;
            }
            flags.btime += flags.binc;
        }

        Color side_to_move = board.side_to_move;
        board = apply_move(&board, result.move);
        if (board.side_to_move == side_to_move)
        {
            printf("Invalid move: %s\n", result.move);
            print_board(&board);
            exit(1);
        }

        push_game_history(board);
        board_state = board_to_board_state(&board);
        print_board(&board);
        printf("Move: %s, Score: %d, Depth: %d, Result: %s\n", result.move, result.score.score, result.score.depth, result.score.result == WON ? "WON" : (result.score.result == LOST ? "LOST" : (result.score.result == DRAW ? "DRAW" : "UNKNOWN")));
        printf("White time: %.1f, Black time: %.1f\n", flags.wtime / 1000.0, flags.btime / 1000.0);

        if (threefold_repetition())
        {
            printf("Draw by threefold repetition\n");
            break;
        }

        if (has_50_move_rule_occurred())
        {
            printf("Draw by 50 move rule\n");
            break;
        }

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
        else if (game_result == DRAW)
        {
            printf("Stalemate\n");
            break;
        }

        printf("\n");
    }
}