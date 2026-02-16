#include "unity.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "utils/board.h"
#include "utils/fen.h"
#include "utils/move.h"

#include "algorithm/game_history.h"
#include "algorithm/zobrist_hash.h"

// Your generated file:
#include "tests/test_threefold_repetition_positions.h"

static void play_moves_and_record_history(Board *board, const char *uci_move_list)
{
    // Copy because strtok modifies the string
    char *buf = _strdup(uci_move_list); // Windows; on Linux use strdup()
    if (!buf)
        TEST_FAIL_MESSAGE("Out of memory duplicating move list");

    char *token = strtok(buf, " ");
    while (token != NULL)
    {
        // token is something like "e2e4", "g1f3", "e7e8q", etc.
        if (!can_move(board, token))
        {
            // Print some context to help debug broken testcases / move parser
            char msg[512];
            snprintf(msg, sizeof(msg), "Illegal move '%s' in sequence: %s", token, uci_move_list);
            free(buf);
            TEST_FAIL_MESSAGE(msg);
        }

        *board = apply_move(board, token);

        uint64_t hash = hash_board(board);
        push_game_history(hash);

        token = strtok(NULL, " ");
        if (token != NULL && threefold_repetition())
        {
            free(buf);
            // this is error
            char msg[1024];
            snprintf(
                msg, sizeof(msg),
                "Unexpected threefold repetition after move '%s' in sequence: %s",
                token, uci_move_list);
            TEST_FAIL_MESSAGE(msg);
        }
    }

    free(buf);
}

static void run_threefold_case(const TestThreefoldRepetitionPosition *pos)
{
    // Start new game from startpos (your dataset currently assumes startpos)
    Board board = fen_to_board(STARTFEN);

    reset_game_history();
    push_game_history(hash_board(&board)); // initial position must be in history

    // Replay move list and record hashes
    play_moves_and_record_history(&board, pos->fen);

    bool got = threefold_repetition();
    bool expected = pos->is_threefold_repetition;

    if (got != expected)
    {
        char msg[1024];
        snprintf(
            msg, sizeof(msg),
            "Threefold mismatch. expected=%s got=%s\nMoves: %s",
            expected ? "true" : "false",
            got ? "true" : "false",
            pos->fen);
        TEST_FAIL_MESSAGE(msg);
    }
}

static void play_moves_and_record_history_with_reset(Board *board, const char *uci_move_list)
{
    // Copy because strtok modifies the string
    char *buf = _strdup(uci_move_list); // Windows; on Linux use strdup()
    if (!buf)
        TEST_FAIL_MESSAGE("Out of memory duplicating move list");

    char *token = strtok(buf, " ");
    while (token != NULL)
    {
        // token is something like "e2e4", "g1f3", "e7e8q", etc.
        if (!can_move(board, token))
        {
            // Print some context to help debug broken testcases / move parser
            char msg[512];
            snprintf(msg, sizeof(msg), "Illegal move '%s' in sequence: %s", token, uci_move_list);
            free(buf);
            TEST_FAIL_MESSAGE(msg);
        }

        Board previous_board = *board;
        *board = apply_move(board, token);
        force_push_game_history(&previous_board, board);

        token = strtok(NULL, " ");
        if (token != NULL && threefold_repetition())
        {
            free(buf);
            // this is error
            char msg[1024];
            snprintf(
                msg, sizeof(msg),
                "Unexpected threefold repetition after move '%s' in sequence: %s",
                token, uci_move_list);
            TEST_FAIL_MESSAGE(msg);
        }
    }

    free(buf);
}

static void run_threefold_case_with_reset(const TestThreefoldRepetitionPosition *pos)
{
    // Start new game from startpos (your dataset currently assumes startpos)
    Board board = fen_to_board(STARTFEN);

    reset_game_history();
    push_game_history(hash_board(&board)); // initial position must be in history

    // Replay move list and record hashes
    play_moves_and_record_history_with_reset(&board, pos->fen);

    bool got = threefold_repetition();
    bool expected = pos->is_threefold_repetition;

    if (got != expected)
    {
        char msg[1024];
        snprintf(
            msg, sizeof(msg),
            "Threefold mismatch. expected=%s got=%s\nMoves: %s",
            expected ? "true" : "false",
            got ? "true" : "false",
            pos->fen);
        TEST_FAIL_MESSAGE(msg);
    }
}

void test_threefold_repetition_wrapper(void)
{
    size_t n = sizeof(test_threefold_repetition_positions) /
               sizeof(test_threefold_repetition_positions[0]);

    for (size_t i = 0; i < n; i++)
    {
        run_threefold_case(&test_threefold_repetition_positions[i]);
        run_threefold_case_with_reset(&test_threefold_repetition_positions[i]);
    }
}