#include "heuristic.h"

bool has_insufficient_material(Board *board)
{
    // Check pawns, rooks, queens - if any exist, material is sufficient
    if (__builtin_popcountll(board->white_pieces.pawns | board->black_pieces.pawns |
                             board->white_pieces.rooks | board->black_pieces.rooks |
                             board->white_pieces.queens | board->black_pieces.queens) > 0)
        return false;

    int white_knights = __builtin_popcountll(board->white_pieces.knights);
    int black_knights = __builtin_popcountll(board->black_pieces.knights);
    int white_bishops = __builtin_popcountll(board->white_pieces.bishops);
    int black_bishops = __builtin_popcountll(board->black_pieces.bishops);

    // King vs King
    if (white_knights + black_knights + white_bishops + black_bishops == 0)
        return true;

    // King + minor piece vs King
    if (white_knights + black_knights + white_bishops + black_bishops == 1)
        return true;

    // King + Bishop vs King + Bishop (need to check if bishops are same color)
    if (white_knights + black_knights == 0 && white_bishops == 1 && black_bishops == 1)
    {
        // Check if bishops are on same colored squares
        bool white_bishop_on_light = (board->white_pieces.bishops & 0b0101010101010101010101010101010101010101010101010101010101010101) > 0;
        bool black_bishop_on_light = (board->black_pieces.bishops & 0b0101010101010101010101010101010101010101010101010101010101010101) > 0;
        return white_bishop_on_light == black_bishop_on_light;
    }
    
    return false;
}

Result get_result(BoardState *board_state, bool is_finished)
{
    Result result = UNKNOWN;
    if (has_insufficient_material(&board_state->board))
        result = DRAW;
    else if (is_finished)
    {
        if (board_state->white_check)
            result = BLACK_WON;
        else if (board_state->black_check)
            result = WHITE_WON;
        else
            result = DRAW;
    }
    return result;
}

BoardScore score_board(BoardState *board_state, uint8_t depth, bool is_finished)
{
    int32_t score = 0;
    Result result = get_result(board_state, is_finished);

    const int piece_values[5] = {100, 320, 330, 500, 900}; // Pawn, Knight, Bishop, Rook, Queen
    uint64_t white_pieces[] = {board_state->board.white_pieces.pawns, board_state->board.white_pieces.knights, board_state->board.white_pieces.bishops, board_state->board.white_pieces.rooks, board_state->board.white_pieces.queens};
    uint64_t black_pieces[] = {board_state->board.black_pieces.pawns, board_state->board.black_pieces.knights, board_state->board.black_pieces.bishops, board_state->board.black_pieces.rooks, board_state->board.black_pieces.queens};

    // Add piece values
    for (int i = 0; i < 5; i++)
        score += __builtin_popcountll(white_pieces[i]) * piece_values[i] - __builtin_popcountll(black_pieces[i]) * piece_values[i];

    return (BoardScore){score, result, depth};
}