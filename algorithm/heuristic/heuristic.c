#include "heuristic.h"
#include "position_score.h"
#include "material_score.h"
#include "king_safety_score.h"
#include "pawn_structure_score.h"
#include "square_control.h"
#include "../../utils/bitboard.h"
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

double get_game_phase(Board *board)
{
    const int PHASE_KNIGHT = 1;
    const int PHASE_BISHOP = 1;
    const int PHASE_ROOK = 2;
    const int PHASE_QUEEN = 4;

    const int MAX_PHASE = 24;

    int phase = 0;

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

    double game_phase = 1 - (double)phase / (double)MAX_PHASE;

    if (game_phase < 0)
        game_phase = 0;
    else if (game_phase > 1)
        game_phase = 1;
    return game_phase;
}

double score_board(BoardState *board_state)
{
    double score = 0;
    double game_phase = get_game_phase(&board_state->board);

    // Material counting
    MaterialFeatures material_features = get_material_features(&board_state->board, game_phase);
    score += get_feature_score(material_features.center_pawns, CENTER_PAWN_MIDDLEGAME, CENTER_PAWN_ENDGAME);
    score += get_feature_score(material_features.bishop_pawns, BISHOP_PAWN_MIDDLEGAME, BISHOP_PAWN_ENDGAME);
    score += get_feature_score(material_features.knight_pawns, KNIGHT_PAWN_MIDDLEGAME, KNIGHT_PAWN_ENDGAME);
    score += get_feature_score(material_features.rook_pawns, ROOK_PAWN_MIDDLEGAME, ROOK_PAWN_ENDGAME);
    score += get_feature_score(material_features.knights, KNIGHT_MIDDLEGAME, KNIGHT_ENDGAME);
    score += get_feature_score(material_features.bishops, BISHOP_MIDDLEGAME, BISHOP_ENDGAME);
    score += get_feature_score(material_features.first_rook, FIRST_ROOK_MIDDLEGAME, FIRST_ROOK_ENDGAME);
    score += get_feature_score(material_features.additional_rooks, ADDITIONAL_ROOK_MIDDLEGAME, ADDITIONAL_ROOK_ENDGAME);
    score += get_feature_score(material_features.first_queen, FIRST_QUEEN_MIDDLEGAME, FIRST_QUEEN_ENDGAME);
    score += get_feature_score(material_features.additional_queens, ADDITIONAL_QUEEN_MIDDLEGAME, ADDITIONAL_QUEEN_ENDGAME);
    score += get_feature_score(material_features.bishop_pair, BISHOP_PAIR_MIDDLEGAME, BISHOP_PAIR_ENDGAME);

    // Positional scoring
    score += get_position_score(&board_state->board, game_phase);

    // King safety scoring
    KingSafetyFeatures king_safety = get_king_safety_features(board_state, game_phase);
    score += get_feature_score(king_safety.castling.can_castle_both_sides, CAN_CASTLE_BOTH_SIDES_MIDDLEGAME, CAN_CASTLE_BOTH_SIDES_ENDGAME);
    score += get_feature_score(king_safety.castling.can_castle_kingside, CAN_CASTLE_KINGSIDE_MIDDLEGAME, CAN_CASTLE_KINGSIDE_ENDGAME);
    score += get_feature_score(king_safety.castling.can_castle_queenside, CAN_CASTLE_QUEENSIDE_MIDDLEGAME, CAN_CASTLE_QUEENSIDE_ENDGAME);
    score += get_feature_score(king_safety.castling.has_castled_kingside, HAS_CASTLED_KINGSIDE_MIDDLEGAME, HAS_CASTLED_KINGSIDE_ENDGAME);
    score += get_feature_score(king_safety.castling.has_castled_queenside, HAS_CASTLED_QUEENSIDE_MIDDLEGAME, HAS_CASTLED_QUEENSIDE_ENDGAME);
    score += get_feature_score(king_safety.pawn_shelter.front_pawn, FRONT_PAWN_BONUS_MIDDLEGAME, FRONT_PAWN_BONUS_ENDGAME);
    score += get_feature_score(king_safety.pawn_shelter.ahead_pawn, AHEAD_PAWN_BONUS_MIDDLEGAME, AHEAD_PAWN_BONUS_ENDGAME);
    score += get_feature_score(king_safety.pawn_shelter.left_pawn, LEFT_PAWN_BONUS_MIDDLEGAME, LEFT_PAWN_BONUS_ENDGAME);
    score += get_feature_score(king_safety.pawn_shelter.right_pawn, RIGHT_PAWN_BONUS_MIDDLEGAME, RIGHT_PAWN_BONUS_ENDGAME);
    score += get_feature_score(king_safety.attacking_king_squares, ATTACKING_KING_SQUARES_MIDDLEGAME, ATTACKING_KING_SQUARES_ENDGAME);

    // Pawn structure scoring
    PawnStructureFeatures pawn_structure_features = get_pawn_structure_features(board_state, game_phase);
    score += get_feature_score(pawn_structure_features.double_pawns, DOUBLE_PAWN_MIDDLEGAME, DOUBLE_PAWN_ENDGAME);
    score += get_feature_score(pawn_structure_features.isolated_pawns, ISOLATED_PAWN_MIDDLEGAME, ISOLATED_PAWN_ENDGAME);
    score += get_feature_score(pawn_structure_features.backward_pawns, BACKWARD_PAWN_MIDDLEGAME, BACKWARD_PAWN_ENDGAME);
    score += get_feature_score(pawn_structure_features.passed_pawns.one_square_left,
                               PASSED_PAWN_MIDDLEGAME[1], PASSED_PAWN_ENDGAME[1]);
    score += get_feature_score(pawn_structure_features.passed_pawns.two_squares_left,
                               PASSED_PAWN_MIDDLEGAME[2], PASSED_PAWN_ENDGAME[2]);
    score += get_feature_score(pawn_structure_features.passed_pawns.three_squares_left,
                               PASSED_PAWN_MIDDLEGAME[3], PASSED_PAWN_ENDGAME[3]);
    score += get_feature_score(pawn_structure_features.passed_pawns.four_squares_left,
                               PASSED_PAWN_MIDDLEGAME[4], PASSED_PAWN_ENDGAME[4]);
    score += get_feature_score(pawn_structure_features.passed_pawns.five_squares_left,
                               PASSED_PAWN_MIDDLEGAME[5], PASSED_PAWN_ENDGAME[5]);
    score += get_feature_score(pawn_structure_features.passed_pawns.six_squares_left,
                               PASSED_PAWN_MIDDLEGAME[6], PASSED_PAWN_ENDGAME[6]);
    score += get_feature_score(pawn_structure_features.pawn_chains.protected_pawns, PROTECTED_PAWN_MIDDLEGAME, PROTECTED_PAWN_ENDGAME);
    score += get_feature_score(pawn_structure_features.pawn_chains.protected_by_protected_pawns, PROTECTED_BY_PROTECTED_PAWN_MIDDLEGAME, PROTECTED_BY_PROTECTED_PAWN_ENDGAME);

    // Square control scoring
    SquareControlFeatures square_control_features = get_square_control_features(board_state, game_phase);
    score += get_feature_score(square_control_features.bishop_my_side_attacks, BISHOP_ATTACKS_MY_SIDE_MIDDLEGAME, BISHOP_ATTACKS_MY_SIDE_ENDGAME);
    score += get_feature_score(square_control_features.bishop_opponent_side_attacks, BISHOP_ATTACKS_OPPONENT_SIDE_MIDDLEGAME, BISHOP_ATTACKS_OPPONENT_SIDE_ENDGAME);
    score += get_feature_score(square_control_features.rook_my_side_attacks, ROOK_ATTACKS_MY_SIDE_MIDDLEGAME, ROOK_ATTACKS_MY_SIDE_ENDGAME);
    score += get_feature_score(square_control_features.rook_opponent_side_attacks, ROOK_ATTACKS_OPPONENT_SIDE_MIDDLEGAME, ROOK_ATTACKS_OPPONENT_SIDE_ENDGAME);
    score += get_feature_score(square_control_features.queen_my_side_attacks, QUEEN_ATTACKS_MY_SIDE_MIDDLEGAME, QUEEN_ATTACKS_MY_SIDE_ENDGAME);
    score += get_feature_score(square_control_features.queen_opponent_side_attacks, QUEEN_ATTACKS_OPPONENT_SIDE_MIDDLEGAME, QUEEN_ATTACKS_OPPONENT_SIDE_ENDGAME);

    if (board_state->board.side_to_move == BLACK)
        score = -score;

    return score;
}