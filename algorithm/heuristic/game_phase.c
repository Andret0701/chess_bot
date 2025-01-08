#include "game_phase.h"

GamePhase get_game_phase(Board *board) // Inspired by lichess' game phase calculation
{
    // Count material for phase calculation excluding kings
    int total_material =
        9 * (popcountll(board->white_pieces.queens) + popcountll(board->black_pieces.queens)) +
        5 * (popcountll(board->white_pieces.rooks) + popcountll(board->black_pieces.rooks)) +
        3 * (popcountll(board->white_pieces.bishops) + popcountll(board->black_pieces.bishops) +
             popcountll(board->white_pieces.knights) + popcountll(board->black_pieces.knights)) +
        popcountll(board->white_pieces.pawns) + popcountll(board->black_pieces.pawns);

    uint64_t white_pieces = board->white_pieces.pawns | board->white_pieces.knights | board->white_pieces.bishops | board->white_pieces.rooks | board->white_pieces.queens;
    uint64_t black_pieces = board->black_pieces.pawns | board->black_pieces.knights | board->black_pieces.bishops | board->black_pieces.rooks | board->black_pieces.queens;

    // Check original back rank pieces for opening phase
    int white_back_rank = 0;
    int black_back_rank = 0;
    for (int i = 0; i < 8; i++)
    {
        if (white_pieces & (1ULL << i))
            white_back_rank++;
        if (black_pieces & (1ULL << (56 + i)))
            black_back_rank++;
    }
    bool queens_present = (popcountll(board->white_pieces.queens) + popcountll(board->black_pieces.queens)) > 0;

    // Early game indicators
    if (total_material >= 62 && // Most pieces still present
        queens_present &&       // Queens still on board
        white_back_rank >= 6 && // Most back rank pieces present
        black_back_rank >= 6)
    {
        return OPENING;
    }

    // Endgame indicators
    if (total_material <= 30 || // Significant material reduction
        !queens_present ||      // No queens
        (popcountll(board->white_pieces.pawns) + popcountll(board->black_pieces.pawns)) <= 8)
    {
        return ENDGAME;
    }

    // Default to midgame
    return MIDGAME;
}