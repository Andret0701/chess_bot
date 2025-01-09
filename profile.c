#include "profile.h"

#include "engine/board_stack.h"
#include "algorithm/bot.h"
#include "fen.h"
#include <time.h>
#include <stdio.h>
#include "algorithm/min_max.h"
#include "move.h"
#include "algorithm/game_history.h"
#include "algorithm/heuristic/heuristic.h"

void play_game(double think_time)
{
    Board board = fen_to_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");

    print_board(&board);
    reset_game_history();
    push_game_history(board);
    BoardState board_state = board_to_board_state(&board);
    while (true)
    {
        clock_t start = clock();
        BotResult result = run_bot(board_to_fen(&board_state.board), think_time);
        clock_t end = clock();
        double time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (time_used > think_time)
        {
            printf("Used to long to think: %f\n", think_time);
            printf("That was %f seconds to much.\n", time_used - think_time);
        }

        board = apply_move(&board, result.move);
        push_game_history(board);
        board_state = board_to_board_state(&board);
        print_board(&board);
        printf("Move: %s, Score: %d, Depth: %d, Result: %s\n", result.move, result.score.score, result.score.depth,
               result.score.result == WHITE_WON ? "White won" : result.score.result == BLACK_WON ? "Black won"
                                                            : result.score.result == DRAW        ? "Draw"
                                                                                                 : "Unknown");

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
        if (game_result == WHITE_WON)
        {
            printf("White checkmated black\n");
            break;
        }
        else if (game_result == BLACK_WON)
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