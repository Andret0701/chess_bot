#include "move_sort.h"
#include <stdio.h>
#include "../engine/encoded_move.h"
#include "move_categorization.h"

#define MAX_DEPTH 255
#define MAX_MOVES 300

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

uint8_t get_mvvlva(BoardState *from, BoardState *to)
{
    const int MVV_LVA[5][6] = {
        {50, 51, 52, 53, 54, 55}, // victim Q, attacker K, Q, R, B, N, P
        {40, 41, 42, 43, 44, 45}, // victim R, attacker K, Q, R, B, N, P
        {30, 31, 32, 33, 34, 35}, // victim B, attacker K, Q, R, B, N, P
        {20, 21, 22, 23, 24, 25}, // victim N, attacker K， Q， R， B， N， P
        {10, 11, 12, 13, 14, 15}, // victim P， attacker K， Q， R， B， N， P
    };

    uint64_t moved_piece;
    uint64_t captured_piece;
    if (from->board.side_to_move == WHITE)
    {
        moved_piece = from->white_pieces & ~to->white_pieces;
        captured_piece = from->black_pieces & ~to->black_pieces;
    }
    else
    {
        moved_piece = from->black_pieces & ~to->black_pieces;
        captured_piece = from->white_pieces & ~to->white_pieces;
    }

    uint8_t moved_piece_index = 0;
    uint8_t captured_piece_index = 0;

    if (from->board.side_to_move == WHITE)
    {
        if (moved_piece & from->board.white_pieces.pawns)
            moved_piece_index = 5;
        else if (moved_piece & from->board.white_pieces.knights)
            moved_piece_index = 4;
        else if (moved_piece & from->board.white_pieces.bishops)
            moved_piece_index = 3;
        else if (moved_piece & from->board.white_pieces.rooks)
            moved_piece_index = 2;
        else if (moved_piece & from->board.white_pieces.queens)
            moved_piece_index = 1;
        else
            moved_piece_index = 0;

        if (captured_piece & from->board.black_pieces.pawns)
            captured_piece_index = 4;
        else if (captured_piece & from->board.black_pieces.knights)
            captured_piece_index = 3;
        else if (captured_piece & from->board.black_pieces.bishops)
            captured_piece_index = 2;
        else if (captured_piece & from->board.black_pieces.rooks)
            captured_piece_index = 1;
        else
            captured_piece_index = 0;
    }
    else
    {
        if (moved_piece & from->board.black_pieces.pawns)
            moved_piece_index = 5;
        else if (moved_piece & from->board.black_pieces.knights)
            moved_piece_index = 4;
        else if (moved_piece & from->board.black_pieces.bishops)
            moved_piece_index = 3;
        else if (moved_piece & from->board.black_pieces.rooks)
            moved_piece_index = 2;
        else if (moved_piece & from->board.black_pieces.queens)
            moved_piece_index = 1;
        else
            moved_piece_index = 0;

        if (captured_piece & from->board.white_pieces.pawns)
            captured_piece_index = 4;
        else if (captured_piece & from->board.white_pieces.knights)
            captured_piece_index = 3;
        else if (captured_piece & from->board.white_pieces.bishops)
            captured_piece_index = 2;
        else if (captured_piece & from->board.white_pieces.rooks)
            captured_piece_index = 1;
        else
            captured_piece_index = 0;
    }

    return MVV_LVA[captured_piece_index][moved_piece_index] * 2 + (is_move_check(to) ? 1 : 0);
}

uint8_t get_quiet_score(BoardState *from, BoardState *to)
{
    uint64_t from_moved_piece;
    uint64_t to_moved_piece;

    if (from->board.side_to_move == WHITE)
    {
        from_moved_piece = from->white_pieces & ~to->white_pieces;
        to_moved_piece = to->white_pieces & ~from->white_pieces;
    }
    else
    {
        from_moved_piece = from->black_pieces & ~to->black_pieces;
        to_moved_piece = to->black_pieces & ~from->black_pieces;
    }

    bool from_attacked;
    bool to_protected;
    bool to_attacked;

    if (from->board.side_to_move == WHITE)
    {
        from_attacked = (from->black_attack & from_moved_piece) != 0;
        to_protected = (to->white_attack & to_moved_piece) != 0;
        to_attacked = (to->black_attack & to_moved_piece) != 0;
    }
    else
    {
        from_attacked = (from->white_attack & from_moved_piece) != 0;
        to_protected = (to->black_attack & to_moved_piece) != 0;
        to_attacked = (to->white_attack & to_moved_piece) != 0;
    }

    uint8_t score = 0;

    if (is_move_check(to))
        score += 8;

    if (is_move_threatening_promotion(from, to))
        score += 10;

    if (from_attacked && !to_attacked)
        score += 6; // escaping attack to safety
    else if (!to_attacked && to_protected)
        score += 5; // safe and defended
    else if (!to_attacked)
        score += 4; // safe but undefended
    else if (from_attacked && to_attacked && to_protected)
        score += 3; // still attacked, but at least defended
    else if (to_attacked && to_protected)
        score += 2; // walks into attacked square, but defended
    else
        score += 0; // attacked and undefended

    return score;
}

void sort_moves(BoardState *from, BoardStack *stack, uint16_t base, uint16_t tt_move, uint8_t depth)
{
    uint16_t num_moves = stack->count - base;
    uint8_t scores[MAX_MOVES];

    for (uint16_t i = 0; i < num_moves; i++)
    {
        BoardState *move_state = &stack->boards[base + i];

        if (encoded_move_equals(move_state->move, tt_move))
            scores[i] = 250;
        else if (is_move_capture(from, move_state))
            scores[i] = get_mvvlva(from, move_state) + 50;
        else if (depth < MAX_DEPTH && encoded_move_equals(move_state->move, killer_moves[depth][0]))
            scores[i] = 49;
        else if (depth < MAX_DEPTH && encoded_move_equals(move_state->move, killer_moves[depth][1]))
            scores[i] = 48;
        else
            scores[i] = get_quiet_score(from, move_state);
    }

    for (uint16_t i = 1; i < num_moves; ++i)
    {
        BoardState key = stack->boards[base + i];
        uint8_t key_score = scores[i];

        int j = i - 1;

        while (j >= 0 && scores[j] < key_score)
        {
            stack->boards[base + j + 1] = stack->boards[base + j];
            scores[j + 1] = scores[j];
            --j;
        }

        stack->boards[base + j + 1] = key;
        scores[j + 1] = key_score;
    }
}

void sort_moves_q(BoardState *from, BoardStack *stack, uint16_t base)
{
    uint16_t num_moves = stack->count - base;
    uint8_t scores[MAX_MOVES];

    for (uint16_t i = 0; i < num_moves; i++)
    {
        BoardState *move_state = &stack->boards[base + i];
        scores[i] = get_mvvlva(from, move_state);
    }

    for (uint16_t i = 1; i < num_moves; ++i)
    {
        BoardState key = stack->boards[base + i];
        uint8_t key_score = scores[i];

        int j = i - 1;

        while (j >= 0 && scores[j] < key_score)
        {
            stack->boards[base + j + 1] = stack->boards[base + j];
            scores[j + 1] = scores[j];
            --j;
        }

        stack->boards[base + j + 1] = key;
        scores[j + 1] = key_score;
    }
}