#include "transposition_table.h"

#define NUM_TT_ENTRIES 400000
#define NUM_DEPTHS 15

TT_Entry transposition_tables[NUM_DEPTHS][2][NUM_TT_ENTRIES] = {0};
uint64_t zobrist_keys[64][12] = {0};
uint64_t zobrist_castling_keys[4] = {0};
uint64_t zobrist_en_passant_keys[8] = {0};

uint64_t rand64()
{
    return ((uint64_t)rand() << 32) | rand();
}

void init_zobrist_keys()
{
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            zobrist_keys[i][j] = rand64();
        }
    }

    for (int i = 0; i < 4; i++)
    {
        zobrist_castling_keys[i] = rand64();
    }

    for (int i = 0; i < 8; i++)
    {
        zobrist_en_passant_keys[i] = rand64();
    }
}

uint64_t hash_board(Board *board)
{
    uint64_t hash = 0;
    for (int i = 0; i < 64; i++)
    {
        if (board->white_pieces.pawns & (1ULL << i))
            hash ^= zobrist_keys[i][0];
        if (board->white_pieces.knights & (1ULL << i))
            hash ^= zobrist_keys[i][1];
        if (board->white_pieces.bishops & (1ULL << i))
            hash ^= zobrist_keys[i][2];
        if (board->white_pieces.rooks & (1ULL << i))
            hash ^= zobrist_keys[i][3];
        if (board->white_pieces.queens & (1ULL << i))
            hash ^= zobrist_keys[i][4];
        if (board->white_pieces.king & (1ULL << i))
            hash ^= zobrist_keys[i][5];
        if (board->black_pieces.pawns & (1ULL << i))
            hash ^= zobrist_keys[i][6];
        if (board->black_pieces.knights & (1ULL << i))
            hash ^= zobrist_keys[i][7];
        if (board->black_pieces.bishops & (1ULL << i))
            hash ^= zobrist_keys[i][8];
        if (board->black_pieces.rooks & (1ULL << i))
            hash ^= zobrist_keys[i][9];
        if (board->black_pieces.queens & (1ULL << i))
            hash ^= zobrist_keys[i][10];
        if (board->black_pieces.king & (1ULL << i))
            hash ^= zobrist_keys[i][11];
    }

    if (board->castling_rights & WHITE_KINGSIDE_CASTLE)
        hash ^= zobrist_castling_keys[0];
    if (board->castling_rights & WHITE_QUEENSIDE_CASTLE)
        hash ^= zobrist_castling_keys[1];
    if (board->castling_rights & BLACK_KINGSIDE_CASTLE)
        hash ^= zobrist_castling_keys[2];
    if (board->castling_rights & BLACK_QUEENSIDE_CASTLE)
        hash ^= zobrist_castling_keys[3];

    if (board->en_passant)
    {
        int ep_square = __builtin_ctzll(board->en_passant);
        hash ^= zobrist_en_passant_keys[ep_square % 8];
    }

    return hash;
}

void update_tt_entry(Board *board, BoardScore score, uint8_t depth, TT_Entry_Type type)
{
    // Compute the hash for the current board position
    uint64_t hash = hash_board(board);
    // Identify the entry in the transposition table
    TT_Entry *entry = &transposition_tables[depth][board->side_to_move][hash % NUM_TT_ENTRIES];

    // If the entry is invalid, replace it directly
    if (!entry->valid)
    {
        entry->valid = true;
        entry->board = *board;
        entry->score = score;
        entry->type = type;
        return;
    }

    // Replacement policy: prioritize exact scores (Maybe check if next entry is valid)
    if (type != EXACT && entry->type == EXACT)
        return;

    entry->board = *board;
    entry->score = score;
    entry->type = type;
}

TT_Entry check_tt_entry(Board *board, uint8_t depth)
{
    // Compute the hash for the current board position
    uint64_t hash = hash_board(board);

    TT_Entry entry = transposition_tables[depth][board->side_to_move][hash % NUM_TT_ENTRIES];
    if (entry.valid && board_equals(board, &entry.board))
        return entry;

    entry.valid = false;
    return entry;
}
