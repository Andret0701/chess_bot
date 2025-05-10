#pragma once

const double PAWN_TABLE_MIDDLEGAME[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5, 5, 10, 25, 25, 10, 5, 5,
    0, 0, 0, 20, 20, 0, 0, 0,
    5, -5, -10, 0, 0, -10, -5, 5,
    5, 10, 10, -20, -20, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0};

const double PAWN_TABLE_ENDGAME[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    80, 80, 80, 80, 80, 80, 80, 80,
    50, 50, 50, 50, 50, 50, 50, 50,
    30, 30, 30, 30, 30, 30, 30, 30,
    20, 20, 20, 20, 20, 20, 20, 20,
    10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10,
    0, 0, 0, 0, 0, 0, 0, 0};

const double ROOK_TABLE_MIDDLEGAME[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 5, 5, 0, 0, 0};

const double ROOK_TABLE_ENDGAME[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 5, 5, 0, 0, 0};

const double KNIGHT_TABLE_MIDDLEGAME[] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50};

const double KNIGHT_TABLE_ENDGAME[] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50};

const double BISHOP_TABLE_MIDDLEGAME[] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -20, -10, -10, -10, -10, -10, -10, -20};

const double BISHOP_TABLE_ENDGAME[] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -20, -10, -10, -10, -10, -10, -10, -20};

const double QUEEN_TABLE_MIDDLEGAME[] = {
    -20, -10, -10, -5, -5, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 5, 5, 5, 0, -10,
    -5, 0, 5, 5, 5, 5, 0, -5,
    0, 0, 5, 5, 5, 5, 0, -5,
    -10, 5, 5, 5, 5, 5, 0, -10,
    -10, 0, 5, 0, 0, 0, 0, -10,
    -20, -10, -10, -5, -5, -10, -10, -20};

const double QUEEN_TABLE_ENDGAME[] = {
    -20, -10, -10, -5, -5, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 5, 5, 5, 0, -10,
    -5, 0, 5, 5, 5, 5, 0, -5,
    0, 0, 5, 5, 5, 5, 0, -5,
    -10, 5, 5, 5, 5, 5, 0, -10,
    -10, 0, 5, 0, 0, 0, 0, -10,
    -20, -10, -10, -5, -5, -10, -10, -20};

const double KING_TABLE_MIDDLEGAME[] = {
    -80, -70, -70, -70, -70, -70, -70, -80,
    -60, -60, -60, -60, -60, -60, -60, -60,
    -40, -50, -50, -60, -60, -50, -50, -40,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    20, 20, -5, -5, -5, -5, 20, 20,
    20, 30, 10, 0, 0, 10, 30, 20};

const double KING_TABLE_ENDGAME[] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -5, 0, 5, 5, 5, 5, 0, -5,
    -10, -5, 20, 30, 30, 20, -5, -10,
    -15, -10, 35, 45, 45, 35, -10, -15,
    -20, -15, 30, 40, 40, 30, -15, -20,
    -25, -20, 20, 25, 25, 20, -20, -25,
    -30, -25, 0, 0, 0, 0, -25, -30,
    -50, -30, -30, -30, -30, -30, -30, -50};

#define DOUBLE_PAWN_MIDDLEGAME -20
#define DOUBLE_PAWN_ENDGAME -20

#define ISOLATED_PAWN_MIDDLEGAME -20
#define ISOLATED_PAWN_ENDGAME -20

#define BACKWARD_PAWN_MIDDLEGAME -15
#define BACKWARD_PAWN_ENDGAME -15

#define PROTECTED_PAWN_MIDDLEGAME 10
#define PROTECTED_PAWN_ENDGAME 10
#define PROTECTED_BY_PROTECTED_PAWN_MIDDLEGAME 10
#define PROTECTED_BY_PROTECTED_PAWN_ENDGAME 10

#define BISHOP_PAWN_FRIENDLY_MIDDLEGAME 3
#define BISHOP_PAWN_FRIENDLY_ENDGAME 3
#define BISHOP_PAWN_ENEMY_MIDDLEGAME 2
#define BISHOP_PAWN_ENEMY_ENDGAME 2

static const double PASSED_PAWN_MIDDLEGAME[] = {0, 120, 80, 50, 30, 15, 15}; // Number of squares from promotion
static const double PASSED_PAWN_ENDGAME[] = {0, 120, 80, 50, 30, 15, 15};    // Number of squares from promotion

#define CAN_CASTLE_KINGSIDE_MIDDLEGAME 20
#define CAN_CASTLE_KINGSIDE_ENDGAME 20

#define CAN_CASTLE_QUEENSIDE_MIDDLEGAME 15
#define CAN_CASTLE_QUEENSIDE_ENDGAME 15

#define CAN_CASTLE_BOTH_SIDES_MIDDLEGAME 25
#define CAN_CASTLE_BOTH_SIDES_ENDGAME 25

#define HAS_CASTLED_KINGSIDE_MIDDLEGAME 30
#define HAS_CASTLED_KINGSIDE_ENDGAME 30

#define HAS_CASTLED_QUEENSIDE_MIDDLEGAME 25
#define HAS_CASTLED_QUEENSIDE_ENDGAME 25

#define FRONT_PAWN_BONUS_MIDDLEGAME 25
#define FRONT_PAWN_BONUS_ENDGAME 25

#define AHEAD_PAWN_BONUS_MIDDLEGAME 5
#define AHEAD_PAWN_BONUS_ENDGAME 5

#define LEFT_PAWN_BONUS_MIDDLEGAME 8
#define LEFT_PAWN_BONUS_ENDGAME 5

#define RIGHT_PAWN_BONUS_MIDDLEGAME 8
#define RIGHT_PAWN_BONUS_ENDGAME 5

static const double ATTACKING_KING_SQUARES_MIDDLEGAME[] = {0, -1, -2, -4, -8, -16, -32, -64, -128}; // Number of squares around king
static const double ATTACKING_KING_SQUARES_ENDGAME[] = {0, -1, -2, -4, -8, -16, -32, -64, -128};    // Number of squares around king

#define BISHOP_ATTACKS_MIDDLEGAME 5
#define BISHOP_ATTACKS_ENDGAME 5
#define ROOK_ATTACKS_MIDDLEGAME 2
#define ROOK_ATTACKS_ENDGAME 2
#define QUEEN_ATTACKS_MIDDLEGAME 1
#define QUEEN_ATTACKS_ENDGAME 1

#define CENTER_PAWN_MIDDLEGAME 100
#define CENTER_PAWN_ENDGAME 100
#define BISHOP_PAWN_MIDDLEGAME 95
#define BISHOP_PAWN_ENDGAME 100
#define KNIGHT_PAWN_MIDDLEGAME 85
#define KNIGHT_PAWN_ENDGAME 100
#define ROOK_PAWN_MIDDLEGAME 70
#define ROOK_PAWN_ENDGAME 100
#define KNIGHT_MIDDLEGAME 320
#define KNIGHT_ENDGAME 320
#define BISHOP_MIDDLEGAME 330
#define BISHOP_ENDGAME 330
#define FIRST_ROOK_MIDDLEGAME 470
#define FIRST_ROOK_ENDGAME 530
#define ADDITIONAL_ROOK_MIDDLEGAME 450
#define ADDITIONAL_ROOK_ENDGAME 500
#define FIRST_QUEEN_MIDDLEGAME 940
#define FIRST_QUEEN_ENDGAME 940
#define ADDITIONAL_QUEEN_MIDDLEGAME 870
#define ADDITIONAL_QUEEN_ENDGAME 870
#define BISHOP_PAIR_MIDDLEGAME 30
#define BISHOP_PAIR_ENDGAME 50