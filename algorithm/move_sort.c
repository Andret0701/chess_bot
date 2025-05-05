#include "move_sort.h"
#include <stdio.h>
#include "../engine/encoded_move.h"

uint16_t get_mvvlva(BoardState *from, BoardState *to)
{
    const int piece_values[6] = {100, 300, 300, 500, 900, 10000}; // Pawn, Knight, Bishop, Rook, Queen, King

    uint64_t moved_piece;
    uint64_t captured_piece;
    if (from->board.side_to_move == WHITE)
    {
        if (from->black_pieces == to->black_pieces)
            return 0;
        moved_piece = from->white_pieces & ~to->white_pieces;
        captured_piece = from->black_pieces & ~to->black_pieces;
    }
    else
    {
        if (from->white_pieces == to->white_pieces)
            return 0;
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

    return piece_values[captured_piece_index] - piece_values[moved_piece_index] + 10000;
}

int compare_boards(const void *left, const void *right)
{
    return ((BoardState *)right)->mvvlva_score - ((BoardState *)left)->mvvlva_score;
}

void sort_moves(BoardState *from, BoardStack *stack, uint16_t base, uint16_t tt_move)
{
    for (uint16_t i = base; i < stack->count; i++)
    {
        if (encoded_move_equals(stack->boards[i].move, tt_move))
            stack->boards[i].mvvlva_score = UINT16_MAX; // Move from transposition table
        else
            stack->boards[i].mvvlva_score = get_mvvlva(from, &stack->boards[i]);
    }

    //  void __cdecl qsort(void *_Base,size_t _NumOfElements,size_t _SizeOfElements,int (__cdecl *_PtFuncCompare)(const void *, const void *));
    qsort(stack->boards + base, stack->count - base, sizeof(BoardState), compare_boards);
}