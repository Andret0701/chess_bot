#include "rook_score.h"

#include "utils/bitboard.h"
#include "algorithm/heuristic/heuristic_values.h"

int32_t get_open_file_score(Board *board, uint8_t middlegame_phase, uint8_t endgame_phase)
{
    int32_t score = 0;

    // Open file for white rooks
    uint64_t white_rooks = board->white_pieces.rooks;
    while (white_rooks)
    {
        int square = __builtin_ctzll(white_rooks);
        uint64_t file_mask = get_file_mask(square);
        if ((board->white_pieces.pawns & file_mask) == 0 && (board->black_pieces.pawns & file_mask) == 0)
            score += OPEN_FILE_MIDDLEGAME * middlegame_phase + OPEN_FILE_ENDGAME * endgame_phase;
        else if ((board->white_pieces.pawns & file_mask) == 0)
            score += SEMI_OPEN_FILE_MIDDLEGAME * middlegame_phase + SEMI_OPEN_FILE_ENDGAME * endgame_phase;
        white_rooks &= white_rooks - 1;
    }

    // Open file for black rooks
    uint64_t black_rooks = board->black_pieces.rooks;
    while (black_rooks)
    {
        int square = __builtin_ctzll(black_rooks);
        uint64_t file_mask = get_file_mask(square);
        if ((board->white_pieces.pawns & file_mask) == 0 && (board->black_pieces.pawns & file_mask) == 0)
            score -= OPEN_FILE_MIDDLEGAME * middlegame_phase + OPEN_FILE_ENDGAME * endgame_phase;
        else if ((board->black_pieces.pawns & file_mask) == 0)
            score -= SEMI_OPEN_FILE_MIDDLEGAME * middlegame_phase + SEMI_OPEN_FILE_ENDGAME * endgame_phase;
        black_rooks &= black_rooks - 1;
    }

    return score;
}

int32_t get_rook_score(BoardState *board_state, uint8_t middlegame_phase, uint8_t endgame_phase)
{
    int32_t score = 0;

    score += get_open_file_score(&board_state->board, middlegame_phase, endgame_phase);

    return score;
}
