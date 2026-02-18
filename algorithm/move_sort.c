#include "move_sort.h"
#include <stdio.h>
#include "../engine/encoded_move.h"

#define MAX_DEPTH 255

uint16_t killer_moves[MAX_DEPTH][2] = {0};

void clear_killer_moves()
{
    for (uint8_t depth = 0; depth < MAX_DEPTH; depth++)
    {
        killer_moves[depth][0] = 0;
        killer_moves[depth][1] = 0;
    }
}

void add_killer_move(uint16_t move, uint8_t depth)
{
    if (move == 0)
        return;
    if (move == killer_moves[depth][0])
        return;

    killer_moves[depth][1] = killer_moves[depth][0];
    killer_moves[depth][0] = move;
}

uint16_t get_mvvlva(BoardState *from, BoardState *to)
{
    const int piece_values[6] = {100, 300, 300, 500, 900, 10000}; // Pawn, Knight, Bishop, Rook, Queen, King

    uint64_t moved_piece;
    uint64_t captured_piece;
    if (from->board.side_to_move == WHITE)
    {
        if (from->black_pieces == to->black_pieces)
            return to->black_check || to->white_check;
        moved_piece = from->white_pieces & ~to->white_pieces;
        captured_piece = from->black_pieces & ~to->black_pieces;
    }
    else
    {
        if (from->white_pieces == to->white_pieces)
            return to->black_check || to->white_check;
        moved_piece = from->black_pieces & ~to->black_pieces;
        captured_piece = from->white_pieces & ~to->white_pieces;
    }

    uint8_t moved_piece_index = 0;
    uint8_t captured_piece_index = 0;

    if (from->board.side_to_move == WHITE)
    {
        if (moved_piece & from->board.white_pieces.pawns)
            moved_piece_index = 0;
        else if (moved_piece & from->board.white_pieces.knights)
            moved_piece_index = 1;
        else if (moved_piece & from->board.white_pieces.bishops)
            moved_piece_index = 2;
        else if (moved_piece & from->board.white_pieces.rooks)
            moved_piece_index = 3;
        else if (moved_piece & from->board.white_pieces.queens)
            moved_piece_index = 4;
        else
            moved_piece_index = 5;

        if (captured_piece & from->board.black_pieces.pawns)
            captured_piece_index = 0;
        else if (captured_piece & from->board.black_pieces.knights)
            captured_piece_index = 1;
        else if (captured_piece & from->board.black_pieces.bishops)
            captured_piece_index = 2;
        else if (captured_piece & from->board.black_pieces.rooks)
            captured_piece_index = 3;
        else if (captured_piece & from->board.black_pieces.queens)
            captured_piece_index = 4;
        else
            captured_piece_index = 5;
    }
    else
    {
        if (moved_piece & from->board.black_pieces.pawns)
            moved_piece_index = 0;
        else if (moved_piece & from->board.black_pieces.knights)
            moved_piece_index = 1;
        else if (moved_piece & from->board.black_pieces.bishops)
            moved_piece_index = 2;
        else if (moved_piece & from->board.black_pieces.rooks)
            moved_piece_index = 3;
        else if (moved_piece & from->board.black_pieces.queens)
            moved_piece_index = 4;
        else
            moved_piece_index = 5;

        if (captured_piece & from->board.white_pieces.pawns)
            captured_piece_index = 0;
        else if (captured_piece & from->board.white_pieces.knights)
            captured_piece_index = 1;
        else if (captured_piece & from->board.white_pieces.bishops)
            captured_piece_index = 2;
        else if (captured_piece & from->board.white_pieces.rooks)
            captured_piece_index = 3;
        else if (captured_piece & from->board.white_pieces.queens)
            captured_piece_index = 4;
        else
            captured_piece_index = 5;
    }

    return piece_values[captured_piece_index] * 1000 - piece_values[moved_piece_index] + 10000;
}

void sort_moves(BoardState *from, BoardStack *stack, uint16_t base, uint16_t tt_move, uint8_t depth)
{
    for (uint16_t i = base; i < stack->count; i++)
    {
        if (encoded_move_equals(stack->boards[i].move, tt_move))
            stack->boards[i].mvvlva_score = UINT16_MAX; // Move from transposition table
        else if (is_move_capture(from, &stack->boards[i]))
            stack->boards[i].mvvlva_score = get_mvvlva(from, &stack->boards[i]);
        else if (encoded_move_equals(stack->boards[i].move, killer_moves[depth][0]))
            stack->boards[i].mvvlva_score = 2; // First killer move
        else if (encoded_move_equals(stack->boards[i].move, killer_moves[depth][1]))
            stack->boards[i].mvvlva_score = 1; // Second killer move
        else
            stack->boards[i].mvvlva_score = 0; // Non-capture move
    }

    uint16_t num_moves = stack->count - base;
    for (uint16_t i = 1; i < num_moves; ++i)
    {
        BoardState key = stack->boards[base + i];
        int j = i - 1;
        while (j >= 0 && stack->boards[base + j].mvvlva_score < key.mvvlva_score)
        {
            stack->boards[base + j + 1] = stack->boards[base + j];
            --j;
        }
        stack->boards[base + j + 1] = key;
    }
}

void sort_moves_q(BoardState *from, BoardStack *stack, uint16_t base, uint16_t tt_move)
{
    for (uint16_t i = base; i < stack->count; i++)
    {
        if (encoded_move_equals(stack->boards[i].move, tt_move))
            stack->boards[i].mvvlva_score = UINT16_MAX; // Move from transposition table
        else
            stack->boards[i].mvvlva_score = get_mvvlva(from, &stack->boards[i]);
    }

    uint16_t num_moves = stack->count - base;
    for (uint16_t i = 1; i < num_moves; ++i)
    {
        BoardState key = stack->boards[base + i];
        int j = i - 1;
        while (j >= 0 && stack->boards[base + j].mvvlva_score < key.mvvlva_score)
        {
            stack->boards[base + j + 1] = stack->boards[base + j];
            --j;
        }
        stack->boards[base + j + 1] = key;
    }
}