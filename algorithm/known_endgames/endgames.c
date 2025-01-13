#include "endgames.h"
#include "../game_history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "endgame_table.h"
#include "../board_score.h"

bool is_known_endgame(BoardState *board_state)
{
    uint8_t piece_count = popcountll(board_state->occupied);
    if (piece_count > 3)
        return false;

    return true;
}

uint8_t get_piece_square_value(uint64_t piece)
{
    return __builtin_ctzll(piece);
}

uint32_t get_endgame_table_index(Board *board)
{
    uint32_t index = 0;

    if (board->side_to_move == WHITE)
        index += 1;

    index += get_piece_square_value(board->white_pieces.king) * 2;
    index += get_piece_square_value(board->black_pieces.king) * 2 * 64;

    uint8_t index_of_last_piece = 0;
    uint8_t last_piece_type = 0;
    if (board->black_pieces.pawns)
    {
        index_of_last_piece = get_piece_square_value(board->black_pieces.pawns);
        last_piece_type = 0;
    }
    else if (board->black_pieces.rooks)
    {
        index_of_last_piece = get_piece_square_value(board->black_pieces.rooks);
        last_piece_type = 1;
    }
    else if (board->black_pieces.queens)
    {
        index_of_last_piece = get_piece_square_value(board->black_pieces.queens);
        last_piece_type = 2;
    }
    else if (board->white_pieces.pawns)
    {
        index_of_last_piece = get_piece_square_value(board->white_pieces.pawns);
        last_piece_type = 3;
    }
    else if (board->white_pieces.rooks)
    {
        index_of_last_piece = get_piece_square_value(board->white_pieces.rooks);
        last_piece_type = 4;
    }
    else if (board->white_pieces.queens)
    {
        index_of_last_piece = get_piece_square_value(board->white_pieces.queens);
        last_piece_type = 5;
    }

    index += index_of_last_piece * 2 * 64 * 64;
    index += last_piece_type * 2 * 64 * 64 * 64;
    return index;
}

BoardScore score_endgame(BoardState *board_state)
{
    if (!is_known_endgame(board_state))
    {
        fprintf(stderr, "Position is not a known endgame\n");
        exit(EXIT_FAILURE);
    }

    if (has_insufficient_material(&board_state->board))
        return (BoardScore){0, DRAW, 0};

    uint32_t index = get_endgame_table_index(&board_state->board);
    return endgame_table[index];
}