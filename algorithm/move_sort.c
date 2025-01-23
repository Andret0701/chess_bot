#include "move_sort.h"
#include <stdio.h>

// Score multiplier to ensure captures are prioritized over positional moves
#define MVV_LVA_MULTIPLIER 100
// Bonus for undefended captures
#define UNDEFENDED_CAPTURE_BONUS 50

uint16_t get_mvv_lva(BoardState *from, BoardState *to)
{
    // Piece values ordered from pawn to king
    static const int piece_values[6] = {100, 300, 300, 500, 900, 10000};

    uint64_t moved_piece, captured_piece;
    bool is_defended;
    const Pieces *attacker_pieces, *victim_pieces;
    uint64_t attacker_all_pieces, victim_all_pieces;
    uint64_t *attack_bitboard;

    // Set up pointers based on side to move
    if (from->board.side_to_move == WHITE)
    {
        attacker_pieces = &from->board.white_pieces;
        victim_pieces = &from->board.black_pieces;
        attacker_all_pieces = from->white_pieces;
        victim_all_pieces = from->black_pieces;
        attack_bitboard = &to->black_attack;
    }
    else
    {
        attacker_pieces = &from->board.black_pieces;
        victim_pieces = &from->board.white_pieces;
        attacker_all_pieces = from->black_pieces;
        victim_all_pieces = from->white_pieces;
        attack_bitboard = &to->white_attack;
    }

    // If no capture occurred, return baseline score
    if (victim_all_pieces == (from->board.side_to_move == WHITE ? to->black_pieces : to->white_pieces))
    {
        return 0;
    }

    moved_piece = attacker_all_pieces & ~(from->board.side_to_move == WHITE ? to->white_pieces : to->black_pieces);
    captured_piece = victim_all_pieces & ~(from->board.side_to_move == WHITE ? to->black_pieces : to->white_pieces);
    is_defended = *attack_bitboard & captured_piece;

    // Determine piece types
    uint8_t moved_piece_type =
        (moved_piece & attacker_pieces->pawns) ? 0 : (moved_piece & attacker_pieces->knights) ? 1
                                                 : (moved_piece & attacker_pieces->bishops)   ? 2
                                                 : (moved_piece & attacker_pieces->rooks)     ? 3
                                                 : (moved_piece & attacker_pieces->queens)    ? 4
                                                                                              : 5;

    uint8_t captured_piece_type =
        (captured_piece & victim_pieces->pawns) ? 0 : (captured_piece & victim_pieces->knights) ? 1
                                                  : (captured_piece & victim_pieces->bishops)   ? 2
                                                  : (captured_piece & victim_pieces->rooks)     ? 3
                                                  : (captured_piece & victim_pieces->queens)    ? 4
                                                                                                : 5;

    // Calculate MVV-LVA score
    // Higher value victims and lower value attackers get higher scores
    int score = piece_values[captured_piece_type] * MVV_LVA_MULTIPLIER -
                piece_values[moved_piece_type];

    // Bonus for undefended captures
    if (!is_defended)
    {
        score += UNDEFENDED_CAPTURE_BONUS;
    }

    return score > 0 ? score : 0;
}

int compare_boards(const void *left, const void *right)
{
    return ((BoardState *)right)->mvvlva_score - ((BoardState *)left)->mvvlva_score;
}

void sort_moves(BoardState *from, BoardStack *stack, uint16_t base)
{
    const size_t num_moves = stack->count - base;
    for (uint16_t i = base; i < stack->count; i++)
    {
        stack->boards[i].mvvlva_score = get_mvv_lva(from, &stack->boards[i]);
    }
    qsort(&stack->boards[base], num_moves, sizeof(BoardState), compare_boards);
}