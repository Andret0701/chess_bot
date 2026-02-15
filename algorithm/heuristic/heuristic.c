#include "heuristic.h"

#include "utils/bitboard.h"

#include "material_score.h"
#include "position_score.h"
#include "pawn_structure_score.h"
#include "king_safety_score.h"
#include "square_control.h"
#include "rook_score.h"
#include "heuristic_values.h"

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
        result = INSUFFICIENT_MATERIAL;
    else if (is_finished)
    {
        if (board_state->white_check || board_state->black_check)
            result = LOST;
        else
            result = STALEMATE;
    }
    return result;
}

int32_t get_result_score(Result result, uint8_t depth)
{
    if (result == LOST)
        return -MATE_SCORE + depth;
    else if (result == WON)
        printf("Error: get_result_score should not be called for WON result\n");
    else if (result == UNKNOWN)
        printf("Error: get_result_score should not be called for UNKNOWN result\n");

    return 0;
}

uint8_t get_middlegame_phase(Board *board)
{
    const uint8_t PHASE_KNIGHT = 1;
    const uint8_t PHASE_BISHOP = 1;
    const uint8_t PHASE_ROOK = 2;
    const uint8_t PHASE_QUEEN = 4;

    const uint8_t MAX_PHASE = 24;

    uint8_t phase = 0;

    // Count remaining white pieces
    phase += PHASE_KNIGHT * __builtin_popcountll(board->white_pieces.knights);
    phase += PHASE_BISHOP * __builtin_popcountll(board->white_pieces.bishops);
    phase += PHASE_ROOK * __builtin_popcountll(board->white_pieces.rooks);
    phase += PHASE_QUEEN * __builtin_popcountll(board->white_pieces.queens);

    // Count remaining black pieces
    phase += PHASE_KNIGHT * __builtin_popcountll(board->black_pieces.knights);
    phase += PHASE_BISHOP * __builtin_popcountll(board->black_pieces.bishops);
    phase += PHASE_ROOK * __builtin_popcountll(board->black_pieces.rooks);
    phase += PHASE_QUEEN * __builtin_popcountll(board->black_pieces.queens);

    if (phase > MAX_PHASE)
        phase = MAX_PHASE;
    return phase;
}

int32_t score_board(BoardState *board_state)
{
    int32_t score = 0;
    uint8_t middlegame_phase = get_middlegame_phase(&board_state->board);
    uint8_t endgame_phase = 24 - middlegame_phase;

    // Material counting
    score += get_material_score(&board_state->board, middlegame_phase, endgame_phase);

    // Positional scoring
    score += get_position_score(&board_state->board, middlegame_phase, endgame_phase);

    // King safety scoring
    score += get_king_safety_score(board_state, middlegame_phase, endgame_phase);

    // Pawn structure scoring
    score += get_pawn_structure_score(board_state, middlegame_phase, endgame_phase);

    // Square control scoring
    score += get_square_control_score(board_state, middlegame_phase, endgame_phase);

    // Connected rooks
    score += 3 * 24 * (board_state->board.white_pieces.rooks & board_state->white_attacks.rooks != 0);
    score -= 3 * 24 * (board_state->board.black_pieces.rooks & board_state->black_attacks.rooks != 0);

    // Rook scoring
    score += get_rook_score(board_state, middlegame_phase, endgame_phase);

    if (board_state->board.side_to_move == BLACK)
        score = -score;

    return score;
}

bool is_winning(int32_t score)
{
    return score >= MATE_SCORE - MAX_DEPTH;
}

bool is_losing(int32_t score)
{
    return score <= -MATE_SCORE + MAX_DEPTH;
}

bool is_mate_score(int32_t score)
{
    return is_winning(score) || is_losing(score);
}

int32_t value_to_tt(int32_t score, uint8_t depth)
{
    if (is_winning(score))
        return score + depth;
    if (is_losing(score))
        return score - depth;
    return score;
}

int32_t value_from_tt(int32_t score, uint8_t depth)
{
    if (is_winning(score))
        return score - depth;
    if (is_losing(score))
        return score + depth;
    return score;
}
