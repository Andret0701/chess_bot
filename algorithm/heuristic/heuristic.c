#include "heuristic.h"
#include "position_score.h"
#include "material_score.h"
#include "king_safety_score.h"
#include "pawn_structure_score.h"
#include "square_control.h"
#include "../../utils/bitboard.h"

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
        if (board_state->white_check || board_state->black_check)
            result = LOST;
        else
            result = DRAW;
    }
    return result;
}

BoardScore score_board(BoardState *board_state, uint8_t depth, bool is_finished)
{
    int score = 0;
    Result result = get_result(board_state, is_finished);

    // Material counting
    score += get_material_score(&board_state->board);

    // Positional scoring
    score += get_position_score(&board_state->board);

    // King safety scoring
    score += get_king_safety_score(board_state);

    // Pawn structure scoring
    score += get_pawn_structure_score(board_state);

    // Square control scoring
    score += get_square_control(board_state);

    // // In score_board():
    // score += __builtin_popcountll(board_state->white_pieces & CENTER_SQUARES_MASK) * 20;
    // score -= __builtin_popcountll(board_state->black_pieces & CENTER_SQUARES_MASK) * 20;
    // score += __builtin_popcountll(board_state->white_pieces & EXTENDED_CENTER_MASK) * 10;
    // score -= __builtin_popcountll(board_state->black_pieces & EXTENDED_CENTER_MASK) * 10;

    // // Bonus for protected pieces
    // score += __builtin_popcountll(board_state->white_attack & board_state->board.white_pieces.queens) * 10;
    // score += __builtin_popcountll(board_state->white_attack & board_state->board.white_pieces.rooks) * 5;
    // score += __builtin_popcountll(board_state->white_attack & board_state->board.white_pieces.bishops) * 5;
    // score += __builtin_popcountll(board_state->white_attack & board_state->board.white_pieces.knights) * 5;
    // score += __builtin_popcountll(board_state->white_attack & board_state->board.white_pieces.pawns) * 2;

    // score -= __builtin_popcountll(board_state->black_attack & board_state->board.black_pieces.queens) * 10;
    // score -= __builtin_popcountll(board_state->black_attack & board_state->board.black_pieces.rooks) * 5;
    // score -= __builtin_popcountll(board_state->black_attack & board_state->board.black_pieces.bishops) * 5;
    // score -= __builtin_popcountll(board_state->black_attack & board_state->board.black_pieces.knights) * 5;
    // score -= __builtin_popcountll(board_state->black_attack & board_state->board.black_pieces.pawns) * 2;

    // // Bonus for attacking the king
    // score += __builtin_popcountll(board_state->white_attack & board_state->board.black_pieces.king) * 20;
    // score -= __builtin_popcountll(board_state->black_attack & board_state->board.white_pieces.king) * 20;
    // // Bonus for having the move
    // if (board_state->board.side_to_move == WHITE)
    //     score += 5;
    // else
    //     score -= 5;

    // // Bonus for rooks on open files
    // uint64_t white_rooks = board_state->board.white_pieces.rooks;
    // uint64_t black_rooks = board_state->board.black_pieces.rooks;
    // uint64_t white_open_files = 0;
    // uint64_t black_open_files = 0;
    // for (int i = 0; i < 8; i++)
    // {
    //     uint64_t file = 0x0101010101010101ULL << i;
    //     if ((white_rooks & file) == 0)
    //         white_open_files |= file;
    //     if ((black_rooks & file) == 0)
    //         black_open_files |= file;
    // }
    // score += __builtin_popcountll(white_open_files & board_state->white_attack & board_state->board.white_pieces.rooks) * 10;
    // score -= __builtin_popcountll(black_open_files & board_state->black_attack & board_state->board.black_pieces.rooks) * 10;

    // // Bonus for connected rooks
    // if ((board_state->board.white_pieces.rooks & board_state->white_attacks.rooks) != 0)
    //     score += 10;
    // if ((board_state->board.black_pieces.rooks & board_state->black_attacks.rooks) != 0)
    //     score -= 10;

    // // King safety
    // // - Bonus for pawn shelter in front of the king
    // // - Penalty for open files or half-open file next to king
    // // Bonus for number of moves for sliding pieces
    // // Taxicab (Distance from queen to king)
    // // Pawn storm
    // // Attacking squares around the king

    if (board_state->board.side_to_move == BLACK)
        score = -score;
    return (BoardScore){score, result, depth};
}