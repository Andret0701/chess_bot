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

// Define piece-square tables for each piece type
static const int16_t PAWN_TABLE[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    100, 100, 100, 100, 100, 100, 100, 100,
    20, 20, 30, 40, 40, 30, 20, 20,
    5, 5, 10, 25, 25, 10, 5, 5,
    0, 0, 0, 20, 20, 0, 0, 0,
    5, -5, -10, 0, 0, -10, -5, 5,
    5, 10, 10, -20, -20, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0};

static const int16_t KNIGHT_TABLE[64] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50};

static const int16_t BISHOP_TABLE[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -20, -10, -10, -10, -10, -10, -10, -20};

static const int16_t ROOK_TABLE[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 5, 5, 0, 0, 0};

static const int16_t QUEEN_TABLE[64] = {
    -20, -10, -10, -5, -5, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 5, 5, 5, 0, -10,
    -5, 0, 5, 5, 5, 5, 0, -5,
    0, 0, 5, 5, 5, 5, 0, -5,
    -10, 5, 5, 5, 5, 5, 0, -10,
    -10, 0, 5, 0, 0, 0, 0, -10,
    -20, -10, -10, -5, -5, -10, -10, -20};

static const int16_t KING_MG_TABLE[64] = { // Middle game king table
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    20, 20, -15, -15, -15, -15, 20, 20,
    20, 30, 10, 0, 0, 10, 30, 20};

static const int16_t KING_EG_TABLE[64] = { // Endgame king table
    -50, -40, -30, -20, -20, -30, -40, -50,
    -30, -20, -10, 0, 0, -10, -20, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -30, 0, 0, 0, 0, -30, -30,
    -50, -30, -30, -30, -30, -30, -30, -50};

// Flip table for getting white's perspective
static const int FLIP[64] = {
    56, 57, 58, 59, 60, 61, 62, 63,
    48, 49, 50, 51, 52, 53, 54, 55,
    40, 41, 42, 43, 44, 45, 46, 47,
    32, 33, 34, 35, 36, 37, 38, 39,
    24, 25, 26, 27, 28, 29, 30, 31,
    16, 17, 18, 19, 20, 21, 22, 23,
    8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7};

// Base piece values remain the same
const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 320;
const int BISHOP_VALUE = 330;
const int ROOK_VALUE = 500;
const int QUEEN_VALUE = 900;

// Evaluate a single piece's position
static int evaluate_piece_position(uint64_t pieces, const int16_t *table, bool is_white)
{
    int score = 0;
    while (pieces)
    {
        int square = __builtin_ctzll(pieces);
        score += table[is_white ? FLIP[square] : square];
        pieces &= pieces - 1;
    }
    return score;
}

// Check if we're in endgame (simplified)
static bool is_endgame(const BoardState *board_state)
{
    // Consider it endgame if:
    // 1. No queens or
    // 2. Both sides have <= 1 minor piece besides king and pawns
    int total_pieces = __builtin_popcountll(board_state->occupied);
    return total_pieces <= 4;
}

BoardScore score_board(BoardState *board_state, uint8_t depth, bool is_finished)
{
    int score = 0;
    Result result = get_result(board_state, is_finished);

    bool endgame = is_endgame(board_state);

    // Material counting
    score += __builtin_popcountll(board_state->board.white_pieces.pawns) * PAWN_VALUE;
    score -= __builtin_popcountll(board_state->board.black_pieces.pawns) * PAWN_VALUE;
    score += __builtin_popcountll(board_state->board.white_pieces.knights) * KNIGHT_VALUE;
    score -= __builtin_popcountll(board_state->board.black_pieces.knights) * KNIGHT_VALUE;
    score += __builtin_popcountll(board_state->board.white_pieces.bishops) * BISHOP_VALUE;
    score -= __builtin_popcountll(board_state->board.black_pieces.bishops) * BISHOP_VALUE;
    score += __builtin_popcountll(board_state->board.white_pieces.rooks) * ROOK_VALUE;
    score -= __builtin_popcountll(board_state->board.black_pieces.rooks) * ROOK_VALUE;
    score += __builtin_popcountll(board_state->board.white_pieces.queens) * QUEEN_VALUE;
    score -= __builtin_popcountll(board_state->board.black_pieces.queens) * QUEEN_VALUE;

    // Positional scoring
    score += evaluate_piece_position(board_state->board.white_pieces.pawns, PAWN_TABLE, true);
    score -= evaluate_piece_position(board_state->board.black_pieces.pawns, PAWN_TABLE, false);
    score += evaluate_piece_position(board_state->board.white_pieces.knights, KNIGHT_TABLE, true);
    score -= evaluate_piece_position(board_state->board.black_pieces.knights, KNIGHT_TABLE, false);
    score += evaluate_piece_position(board_state->board.white_pieces.bishops, BISHOP_TABLE, true);
    score -= evaluate_piece_position(board_state->board.black_pieces.bishops, BISHOP_TABLE, false);
    score += evaluate_piece_position(board_state->board.white_pieces.rooks, ROOK_TABLE, true);
    score -= evaluate_piece_position(board_state->board.black_pieces.rooks, ROOK_TABLE, false);
    score += evaluate_piece_position(board_state->board.white_pieces.queens, QUEEN_TABLE, true);
    score -= evaluate_piece_position(board_state->board.black_pieces.queens, QUEEN_TABLE, false);

    // King positioning (different for middlegame/endgame)
    const int16_t *king_table = endgame ? KING_EG_TABLE : KING_MG_TABLE;
    score += evaluate_piece_position(board_state->board.white_pieces.king, king_table, true);
    score -= evaluate_piece_position(board_state->board.black_pieces.king, king_table, false);

    // Score for where different pieces can attack to
    score += __builtin_popcountll(board_state->white_attacks.pawns) * 2;
    score -= __builtin_popcountll(board_state->black_attacks.pawns) * 2;
    score += __builtin_popcountll(board_state->white_attacks.knights) * 4;
    score -= __builtin_popcountll(board_state->black_attacks.knights) * 4;
    score += __builtin_popcountll(board_state->white_attacks.bishops) * 4;
    score -= __builtin_popcountll(board_state->black_attacks.bishops) * 4;
    score += __builtin_popcountll(board_state->white_attacks.rooks) * 6;
    score -= __builtin_popcountll(board_state->black_attacks.rooks) * 6;
    score += __builtin_popcountll(board_state->white_attacks.queens) * 10;
    score -= __builtin_popcountll(board_state->black_attacks.queens) * 10;

    // At the top with other constants:
    static const uint64_t CENTER_SQUARES = 0x0000001818000000ULL;  // e4,d4,e5,d5
    static const uint64_t EXTENDED_CENTER = 0x00003C3C3C3C0000ULL; // c3-f3 to c6-f6

    // In score_board():
    score += __builtin_popcountll(board_state->white_pieces & CENTER_SQUARES) * 20;
    score -= __builtin_popcountll(board_state->black_pieces & CENTER_SQUARES) * 20;
    score += __builtin_popcountll(board_state->white_pieces & EXTENDED_CENTER) * 10;
    score -= __builtin_popcountll(board_state->black_pieces & EXTENDED_CENTER) * 10;

    // Add this near the pawn evaluation:
    uint64_t white_pawn_attacks = board_state->white_attacks.pawns;
    uint64_t black_pawn_attacks = board_state->black_attacks.pawns;

    // Bonus for pawns protected by other pawns
    score += __builtin_popcountll(board_state->board.white_pieces.pawns & white_pawn_attacks) * 15;
    score -= __builtin_popcountll(board_state->board.black_pieces.pawns & black_pawn_attacks) * 15;

    // Bonus for protected pieces
    score += __builtin_popcountll(board_state->white_attack & board_state->board.white_pieces.queens) * 10;
    score += __builtin_popcountll(board_state->white_attack & board_state->board.white_pieces.rooks) * 5;
    score += __builtin_popcountll(board_state->white_attack & board_state->board.white_pieces.bishops) * 5;
    score += __builtin_popcountll(board_state->white_attack & board_state->board.white_pieces.knights) * 5;
    score += __builtin_popcountll(board_state->white_attack & board_state->board.white_pieces.pawns) * 2;

    score -= __builtin_popcountll(board_state->black_attack & board_state->board.black_pieces.queens) * 10;
    score -= __builtin_popcountll(board_state->black_attack & board_state->board.black_pieces.rooks) * 5;
    score -= __builtin_popcountll(board_state->black_attack & board_state->board.black_pieces.bishops) * 5;
    score -= __builtin_popcountll(board_state->black_attack & board_state->board.black_pieces.knights) * 5;
    score -= __builtin_popcountll(board_state->black_attack & board_state->board.black_pieces.pawns) * 2;

    // Bonus for attacking the king
    score += __builtin_popcountll(board_state->white_attack & board_state->board.black_pieces.king) * 20;
    score -= __builtin_popcountll(board_state->black_attack & board_state->board.white_pieces.king) * 20;

    // Bonus for castling
    if (board_state->board.castling_rights & WHITE_KINGSIDE_CASTLE)
        score += 10;
    if (board_state->board.castling_rights & WHITE_QUEENSIDE_CASTLE)
        score += 10;
    if (board_state->board.castling_rights & BLACK_KINGSIDE_CASTLE)
        score -= 10;
    if (board_state->board.castling_rights & BLACK_QUEENSIDE_CASTLE)
        score -= 10;

    // Bonus for having the move
    if (board_state->board.side_to_move == WHITE)
        score += 5;
    else
        score -= 5;

    // Bonus for bishop pair
    if (__builtin_popcountll(board_state->board.white_pieces.bishops) >= 2)
        score += 20;
    if (__builtin_popcountll(board_state->board.black_pieces.bishops) >= 2)
        score -= 20;

    // Bonus for rooks on open files
    uint64_t white_rooks = board_state->board.white_pieces.rooks;
    uint64_t black_rooks = board_state->board.black_pieces.rooks;
    uint64_t white_open_files = 0;
    uint64_t black_open_files = 0;
    for (int i = 0; i < 8; i++)
    {
        uint64_t file = 0x0101010101010101ULL << i;
        if ((white_rooks & file) == 0)
            white_open_files |= file;
        if ((black_rooks & file) == 0)
            black_open_files |= file;
    }
    score += __builtin_popcountll(white_open_files & board_state->white_attack & board_state->board.white_pieces.rooks) * 10;
    score -= __builtin_popcountll(black_open_files & board_state->black_attack & board_state->board.black_pieces.rooks) * 10;

    // Bonus for connected rooks
    if ((board_state->board.white_pieces.rooks & board_state->white_attacks.rooks) != 0)
        score += 10;
    if ((board_state->board.black_pieces.rooks & board_state->black_attacks.rooks) != 0)
        score -= 10;

    return (BoardScore){score, result, depth};
}