#include "unity.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "utils/board.h"
#include "utils/fen.h"
#include "utils/move.h"

#include "engine/board_state.h"

#include "algorithm/game_history.h"
#include "algorithm/zobrist_hash.h"
#include "algorithm/bot.h"
#include "algorithm/heuristic/heuristic.h"

#include "tests/test_positions.h"
#include "tests/test_threefold_repetition_positions.h"

void test_search(const TestThreefoldRepetitionPosition *pos)
{
    Board board = fen_to_board(STARTFEN);
    BotResult previous_result = {NULL, 0, 0, 0};

    reset_game_history();
    push_game_history(hash_board(&board));

    // Copy because strtok modifies the string
    char *buf = _strdup(pos->fen); // Windows; on Linux use strdup()
    if (!buf)
        TEST_FAIL_MESSAGE("Out of memory duplicating move list");

    char *token = strtok(buf, " ");
    char *previous_token = NULL;
    while (token != NULL)
    {
        // token is something like "e2e4", "g1f3", "e7e8q", etc.
        if (!can_move(&board, token))
        {
            // Print some context to help debug broken testcases / move parser
            char msg[512];
            snprintf(msg, sizeof(msg), "Illegal move '%s' in sequence: %s", token, pos->fen);
            free(buf);
            TEST_FAIL_MESSAGE(msg);
        }

        uint16_t move_count = get_move_count();
        // BotResult result = run_depth_bot(board, 4);
        // BotResult result = run_nodes_bot(board, 1000);
        BotResult result = run_time_bot(board, 1000, 1000, 0, 0);
        if (move_count != get_move_count())
        {
            char msg[1024];
            snprintf(
                msg, sizeof(msg),
                "Move count mismatch after move '%s' in sequence: %s. Expected move count: %d, got: %d",
                token, pos->fen, move_count, get_move_count());
            free(buf);
            TEST_FAIL_MESSAGE(msg);
        }

        Board previous_board = board;
        board = apply_move(&board, token);
        force_push_game_history(&previous_board, &board);

        char *next_token = strtok(NULL, " ");
        if (next_token == NULL && pos->is_threefold_repetition)
        {
            if ((strcmp(result.move, token) == 0 && result.score != 0) || (strcmp(previous_result.move, previous_token) == 0 && previous_result.score > 0))
            {
                char fen[1024];
                snprintf(fen, sizeof(fen), "%s", board_to_fen(&previous_board));
                char msg[1024];
                snprintf(
                    msg, sizeof(msg),
                    "Threefold repetition detected after move '%s' in sequence: %s, but bot did not return a score of 0. Bot move: %s, Bot score: %s, Previous score: %s, FEN: %s, History move count: %d, Is threefold repetition: %s",
                    token, pos->fen, result.move, format_score(result.score), format_score(previous_result.score), fen, get_move_count(), threefold_repetition() ? "true" : "false");
                free(buf);
                TEST_FAIL_MESSAGE(msg);
            }
        }

        previous_result = result;
        previous_token = token;
        token = next_token;
    }

    free(buf);
}

void test_search_wrapper(void)
{
    size_t n = sizeof(test_threefold_repetition_positions) /
               sizeof(test_threefold_repetition_positions[0]);

    for (size_t i = 0; i < n; i++)
    {
        test_search(&test_threefold_repetition_positions[i]);
    }
}