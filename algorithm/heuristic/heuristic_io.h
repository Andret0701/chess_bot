#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../../utils/board.h"
#include "../board_score.h"
#include "../../engine/board_state.h"
#include "feature.h"
#include "heuristic.h"
#include "position_score.h"
#include "material_score.h"
#include "king_safety_score.h"
#include "pawn_structure_score.h"
#include "square_control.h"
#include "../../utils/fen.h"

void print_features(BoardState *board_state)
{
    double game_phase = get_game_phase(&board_state->board);

    MaterialFeatures material_features = get_material_features(&board_state->board, game_phase);
    PositionFeatures position_features = get_position_features(&board_state->board, game_phase);
    KingSafetyFeatures king_safety_features = get_king_safety_features(board_state, game_phase);
    PawnStructureFeatures pawn_structure_features = get_pawn_structure_features(board_state, game_phase);
    SquareControlFeatures square_control_features = get_square_control_features(board_state, game_phase);

    printf("{");

    // --- Material features ---
    printf("\"CENTER_PAWN_MIDDLEGAME\":%f, ", material_features.center_pawns.middlegame);
    printf("\"CENTER_PAWN_ENDGAME\":%f, ", material_features.center_pawns.endgame);

    printf("\"BISHOP_PAWN_MIDDLEGAME\":%f, ", material_features.bishop_pawns.middlegame);
    printf("\"BISHOP_PAWN_ENDGAME\":%f, ", material_features.bishop_pawns.endgame);

    printf("\"KNIGHT_PAWN_MIDDLEGAME\":%f, ", material_features.knight_pawns.middlegame);
    printf("\"KNIGHT_PAWN_ENDGAME\":%f, ", material_features.knight_pawns.endgame);

    printf("\"ROOK_PAWN_MIDDLEGAME\":%f, ", material_features.rook_pawns.middlegame);
    printf("\"ROOK_PAWN_ENDGAME\":%f, ", material_features.rook_pawns.endgame);

    printf("\"KNIGHT_MIDDLEGAME\":%f, ", material_features.knights.middlegame);
    printf("\"KNIGHT_ENDGAME\":%f, ", material_features.knights.endgame);

    printf("\"BISHOP_MIDDLEGAME\":%f, ", material_features.bishops.middlegame);
    printf("\"BISHOP_ENDGAME\":%f, ", material_features.bishops.endgame);

    printf("\"FIRST_ROOK_MIDDLEGAME\":%f, ", material_features.first_rook.middlegame);
    printf("\"FIRST_ROOK_ENDGAME\":%f, ", material_features.first_rook.endgame);

    printf("\"ADDITIONAL_ROOK_MIDDLEGAME\":%f, ", material_features.additional_rooks.middlegame);
    printf("\"ADDITIONAL_ROOK_ENDGAME\":%f, ", material_features.additional_rooks.endgame);

    printf("\"FIRST_QUEEN_MIDDLEGAME\":%f, ", material_features.first_queen.middlegame);
    printf("\"FIRST_QUEEN_ENDGAME\":%f, ", material_features.first_queen.endgame);

    printf("\"ADDITIONAL_QUEEN_MIDDLEGAME\":%f, ", material_features.additional_queens.middlegame);
    printf("\"ADDITIONAL_QUEEN_ENDGAME\":%f, ", material_features.additional_queens.endgame);

    printf("\"BISHOP_PAIR_MIDDLEGAME\":%f, ", material_features.bishop_pair.middlegame);
    printf("\"BISHOP_PAIR_ENDGAME\":%f, ", material_features.bishop_pair.endgame);

    // --- Piece-square tables (features) ---
    // PAWNS
    printf("\"PAWN_TABLE_MIDDLEGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.pawns[i].middlegame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    printf("\"PAWN_TABLE_ENDGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.pawns[i].endgame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    // KNIGHTS
    printf("\"KNIGHT_TABLE_MIDDLEGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.knights[i].middlegame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    printf("\"KNIGHT_TABLE_ENDGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.knights[i].endgame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    // BISHOPS
    printf("\"BISHOP_TABLE_MIDDLEGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.bishops[i].middlegame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    printf("\"BISHOP_TABLE_ENDGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.bishops[i].endgame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    // ROOKS
    printf("\"ROOK_TABLE_MIDDLEGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.rooks[i].middlegame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    printf("\"ROOK_TABLE_ENDGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.rooks[i].endgame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    // QUEENS
    printf("\"QUEEN_TABLE_MIDDLEGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.queens[i].middlegame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    printf("\"QUEEN_TABLE_ENDGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.queens[i].endgame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    // KINGS
    printf("\"KING_TABLE_MIDDLEGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.king[i].middlegame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    printf("\"KING_TABLE_ENDGAME\":[");
    for (int i = 0; i < 64; i++)
    {
        printf("%f", position_features.king[i].endgame);
        if (i < 63)
            printf(", ");
    }
    printf("], ");

    // --- Pawn structure features ---
    printf("\"DOUBLE_PAWN_MIDDLEGAME\":%f, ", pawn_structure_features.double_pawns.middlegame);
    printf("\"DOUBLE_PAWN_ENDGAME\":%f, ", pawn_structure_features.double_pawns.endgame);

    printf("\"ISOLATED_PAWN_MIDDLEGAME\":%f, ", pawn_structure_features.isolated_pawns.middlegame);
    printf("\"ISOLATED_PAWN_ENDGAME\":%f, ", pawn_structure_features.isolated_pawns.endgame);

    printf("\"BACKWARD_PAWN_MIDDLEGAME\":%f, ", pawn_structure_features.backward_pawns.middlegame);
    printf("\"BACKWARD_PAWN_ENDGAME\":%f, ", pawn_structure_features.backward_pawns.endgame);

    // Arrays for passed pawns: index 0 is dummy 0 to match heuristic table layout
    printf("\"PASSED_PAWN_MIDDLEGAME\":[0.0,%f,%f,%f,%f,%f,%f], ",
           pawn_structure_features.passed_pawns.one_square_left.middlegame,
           pawn_structure_features.passed_pawns.two_squares_left.middlegame,
           pawn_structure_features.passed_pawns.three_squares_left.middlegame,
           pawn_structure_features.passed_pawns.four_squares_left.middlegame,
           pawn_structure_features.passed_pawns.five_squares_left.middlegame,
           pawn_structure_features.passed_pawns.six_squares_left.middlegame);

    printf("\"PASSED_PAWN_ENDGAME\":[0.0,%f,%f,%f,%f,%f,%f], ",
           pawn_structure_features.passed_pawns.one_square_left.endgame,
           pawn_structure_features.passed_pawns.two_squares_left.endgame,
           pawn_structure_features.passed_pawns.three_squares_left.endgame,
           pawn_structure_features.passed_pawns.four_squares_left.endgame,
           pawn_structure_features.passed_pawns.five_squares_left.endgame,
           pawn_structure_features.passed_pawns.six_squares_left.endgame);

    printf("\"PROTECTED_PAWN_MIDDLEGAME\":%f, ",
           pawn_structure_features.pawn_chains.protected_pawns.middlegame);
    printf("\"PROTECTED_PAWN_ENDGAME\":%f, ",
           pawn_structure_features.pawn_chains.protected_pawns.endgame);

    printf("\"PROTECTED_BY_PROTECTED_PAWN_MIDDLEGAME\":%f, ",
           pawn_structure_features.pawn_chains.protected_by_protected_pawns.middlegame);
    printf("\"PROTECTED_BY_PROTECTED_PAWN_ENDGAME\":%f, ",
           pawn_structure_features.pawn_chains.protected_by_protected_pawns.endgame);

    // --- King safety features ---
    // Castling
    printf("\"CAN_CASTLE_BOTH_SIDES_MIDDLEGAME\":%f, ",
           king_safety_features.castling.can_castle_both_sides.middlegame);
    printf("\"CAN_CASTLE_BOTH_SIDES_ENDGAME\":%f, ",
           king_safety_features.castling.can_castle_both_sides.endgame);

    printf("\"CAN_CASTLE_KINGSIDE_MIDDLEGAME\":%f, ",
           king_safety_features.castling.can_castle_kingside.middlegame);
    printf("\"CAN_CASTLE_KINGSIDE_ENDGAME\":%f, ",
           king_safety_features.castling.can_castle_kingside.endgame);

    printf("\"CAN_CASTLE_QUEENSIDE_MIDDLEGAME\":%f, ",
           king_safety_features.castling.can_castle_queenside.middlegame);
    printf("\"CAN_CASTLE_QUEENSIDE_ENDGAME\":%f, ",
           king_safety_features.castling.can_castle_queenside.endgame);

    printf("\"HAS_CASTLED_KINGSIDE_MIDDLEGAME\":%f, ",
           king_safety_features.castling.has_castled_kingside.middlegame);
    printf("\"HAS_CASTLED_KINGSIDE_ENDGAME\":%f, ",
           king_safety_features.castling.has_castled_kingside.endgame);

    printf("\"HAS_CASTLED_QUEENSIDE_MIDDLEGAME\":%f, ",
           king_safety_features.castling.has_castled_queenside.middlegame);
    printf("\"HAS_CASTLED_QUEENSIDE_ENDGAME\":%f, ",
           king_safety_features.castling.has_castled_queenside.endgame);

    // Pawn shelter
    printf("\"FRONT_PAWN_BONUS_MIDDLEGAME\":%f, ",
           king_safety_features.pawn_shelter.front_pawn.middlegame);
    printf("\"FRONT_PAWN_BONUS_ENDGAME\":%f, ",
           king_safety_features.pawn_shelter.front_pawn.endgame);

    printf("\"AHEAD_PAWN_BONUS_MIDDLEGAME\":%f, ",
           king_safety_features.pawn_shelter.ahead_pawn.middlegame);
    printf("\"AHEAD_PAWN_BONUS_ENDGAME\":%f, ",
           king_safety_features.pawn_shelter.ahead_pawn.endgame);

    printf("\"LEFT_PAWN_BONUS_MIDDLEGAME\":%f, ",
           king_safety_features.pawn_shelter.left_pawn.middlegame);
    printf("\"LEFT_PAWN_BONUS_ENDGAME\":%f, ",
           king_safety_features.pawn_shelter.left_pawn.endgame);

    printf("\"RIGHT_PAWN_BONUS_MIDDLEGAME\":%f, ",
           king_safety_features.pawn_shelter.right_pawn.middlegame);
    printf("\"RIGHT_PAWN_BONUS_ENDGAME\":%f, ",
           king_safety_features.pawn_shelter.right_pawn.endgame);

    printf("\"ATTACKING_KING_SQUARES_MIDDLEGAME\":%f, ",
           king_safety_features.attacking_king_squares.middlegame);
    printf("\"ATTACKING_KING_SQUARES_ENDGAME\":%f, ",
           king_safety_features.attacking_king_squares.endgame);

    // --- Square control features ---
    printf("\"BISHOP_ATTACKS_MY_SIDE_MIDDLEGAME\":%f, ",
           square_control_features.bishop_my_side_attacks.middlegame);
    printf("\"BISHOP_ATTACKS_MY_SIDE_ENDGAME\":%f, ",
           square_control_features.bishop_my_side_attacks.endgame);

    printf("\"BISHOP_ATTACKS_OPPONENT_SIDE_MIDDLEGAME\":%f, ",
           square_control_features.bishop_opponent_side_attacks.middlegame);
    printf("\"BISHOP_ATTACKS_OPPONENT_SIDE_ENDGAME\":%f, ",
           square_control_features.bishop_opponent_side_attacks.endgame);

    printf("\"ROOK_ATTACKS_MY_SIDE_MIDDLEGAME\":%f, ",
           square_control_features.rook_my_side_attacks.middlegame);
    printf("\"ROOK_ATTACKS_MY_SIDE_ENDGAME\":%f, ",
           square_control_features.rook_my_side_attacks.endgame);

    printf("\"ROOK_ATTACKS_OPPONENT_SIDE_MIDDLEGAME\":%f, ",
           square_control_features.rook_opponent_side_attacks.middlegame);
    printf("\"ROOK_ATTACKS_OPPONENT_SIDE_ENDGAME\":%f, ",
           square_control_features.rook_opponent_side_attacks.endgame);

    printf("\"QUEEN_ATTACKS_MY_SIDE_MIDDLEGAME\":%f, ",
           square_control_features.queen_my_side_attacks.middlegame);
    printf("\"QUEEN_ATTACKS_MY_SIDE_ENDGAME\":%f, ",
           square_control_features.queen_my_side_attacks.endgame);

    // Last key: no trailing comma
    printf("\"QUEEN_ATTACKS_OPPONENT_SIDE_MIDDLEGAME\":%f, ",
           square_control_features.queen_opponent_side_attacks.middlegame);
    printf("\"QUEEN_ATTACKS_OPPONENT_SIDE_ENDGAME\":%f",
           square_control_features.queen_opponent_side_attacks.endgame);

    printf("}\n");

    fflush(stdout);

    free_position_features(position_features);
}

void print_features_for_board(Board *board)
{
    BoardState board_state = board_to_board_state(board);
    print_features(&board_state);
}
