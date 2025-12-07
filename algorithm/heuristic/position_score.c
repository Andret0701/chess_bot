#include "position_score.h"
#include "heuristic_values.h"

// Evaluate a single piece's position
static double evaluate_piece_position(uint64_t pieces, const double *middlegame_table, const double *endgame_table, bool is_white, double game_phase)
{
    if (is_white)
        pieces = flip_bitboard(pieces);
    double score = 0;
    while (pieces)
    {
        int square = __builtin_ctzll(pieces);
        score += middlegame_table[square] * (1 - game_phase) + endgame_table[square] * game_phase;
        pieces &= pieces - 1;
    }
    return score;
}

double get_position_score(Board *board, double game_phase)
{
    double score = 0;

    score += evaluate_piece_position(board->white_pieces.pawns, PAWN_TABLE_MIDDLEGAME, PAWN_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.pawns, PAWN_TABLE_MIDDLEGAME, PAWN_TABLE_ENDGAME, false, game_phase);
    score += evaluate_piece_position(board->white_pieces.king, KING_TABLE_MIDDLEGAME, KING_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.king, KING_TABLE_MIDDLEGAME, KING_TABLE_ENDGAME, false, game_phase);
    score += evaluate_piece_position(board->white_pieces.knights, KNIGHT_TABLE_MIDDLEGAME, KNIGHT_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.knights, KNIGHT_TABLE_MIDDLEGAME, KNIGHT_TABLE_ENDGAME, false, game_phase);
    score += evaluate_piece_position(board->white_pieces.bishops, BISHOP_TABLE_MIDDLEGAME, BISHOP_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.bishops, BISHOP_TABLE_MIDDLEGAME, BISHOP_TABLE_ENDGAME, false, game_phase);
    score += evaluate_piece_position(board->white_pieces.rooks, ROOK_TABLE_MIDDLEGAME, ROOK_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.rooks, ROOK_TABLE_MIDDLEGAME, ROOK_TABLE_ENDGAME, false, game_phase);
    score += evaluate_piece_position(board->white_pieces.queens, QUEEN_TABLE_MIDDLEGAME, QUEEN_TABLE_ENDGAME, true, game_phase);
    score -= evaluate_piece_position(board->black_pieces.queens, QUEEN_TABLE_MIDDLEGAME, QUEEN_TABLE_ENDGAME, false, game_phase);

    return score;
}

static Feature *get_piece_square_features(uint64_t white_pieces, uint64_t black_pieces, double game_phase)
{
    Feature *features = malloc(64 * sizeof(Feature));
    white_pieces = flip_bitboard(white_pieces);

    for (int square = 0; square < 64; square++)
    {
        features[square].middlegame = 0;
        features[square].endgame = 0;

        if (white_pieces & (1ULL << square))
        {
            features[square].middlegame += (1 - game_phase);
            features[square].endgame += game_phase;
        }
        if (black_pieces & (1ULL << square))
        {
            features[square].middlegame -= (1 - game_phase);
            features[square].endgame -= game_phase;
        }
    }
    return features;
}

static void free_piece_square_features(Feature *features)
{
    free(features);
}

PositionFeatures get_position_features(Board *board, double game_phase)
{
    PositionFeatures position_features;

    position_features.pawns = get_piece_square_features(board->white_pieces.pawns, board->black_pieces.pawns, game_phase);
    position_features.knights = get_piece_square_features(board->white_pieces.knights, board->black_pieces.knights, game_phase);
    position_features.bishops = get_piece_square_features(board->white_pieces.bishops, board->black_pieces.bishops, game_phase);
    position_features.rooks = get_piece_square_features(board->white_pieces.rooks, board->black_pieces.rooks, game_phase);
    position_features.queens = get_piece_square_features(board->white_pieces.queens, board->black_pieces.queens, game_phase);
    position_features.king = get_piece_square_features(board->white_pieces.king, board->black_pieces.king, game_phase);

    return position_features;
}

void free_position_features(PositionFeatures position_features)
{
    free_piece_square_features(position_features.pawns);
    free_piece_square_features(position_features.knights);
    free_piece_square_features(position_features.bishops);
    free_piece_square_features(position_features.rooks);
    free_piece_square_features(position_features.queens);
    free_piece_square_features(position_features.king);
}