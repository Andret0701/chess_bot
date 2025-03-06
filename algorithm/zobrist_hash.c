#include "zobrist_hash.h"

#define NUM_PIECE_TYPES 12
#define NUM_SQUARES 64
enum
{
    WHITE_PAWN = 0,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
};

uint64_t zobrist_table[NUM_PIECE_TYPES][NUM_SQUARES];
uint64_t zobrist_castling[16];
uint64_t zobrist_en_passant[NUM_SQUARES];
uint64_t zobrist_black_to_move;

static uint64_t rng_state;

// Seed the PRNG once (ideally at startup)
void seed_rng(uint64_t seed)
{
    // Avoid a zero seed as xorshift algorithms require a nonzero state.
    rng_state = (seed != 0) ? seed : 88172645463325252ULL;
}

// Xorshift64 function for generating 64-bit random numbers.
uint64_t xorshift64()
{
    uint64_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    rng_state = x;
    return x;
}
uint64_t random_64bit()
{
    uint64_t r = 0;
    for (int i = 0; i < 64; i++)
    {
        r |= (xorshift64() & 1ULL) << i;
    }
    return r;
}

void init_zobrist_keys()
{
    // init rand
    srand(time(NULL));
    seed_rng((uint64_t)time(NULL) + rand());

    // Initialize table for each piece type and square.
    for (int piece = 0; piece < NUM_PIECE_TYPES; piece++)
    {
        for (int sq = 0; sq < NUM_SQUARES; sq++)
        {
            zobrist_table[piece][sq] = random_64bit();
        }
    }

    // Initialize castling rights random numbers.
    for (int i = 0; i < 16; i++)
    {
        zobrist_castling[i] = random_64bit();
    }

    // Initialize en passant values.
    // Often only the file matters, but here we assign one for each square.
    for (int sq = 0; sq < NUM_SQUARES; sq++)
    {
        zobrist_en_passant[sq] = random_64bit();
    }

    // Initialize black to move.
    zobrist_black_to_move = random_64bit();
}

uint64_t hash_board(Board *board)
{
    uint64_t hash = 0;

// Helper macro to process a bitboard.
// We'll use __builtin_ctzll (GCC/Clang) to get the index of the least-significant 1-bit.
// If you're using another compiler, you might need to write an alternative.
#define PROCESS_BITBOARD(bitboard, pieceIndex)       \
    do                                               \
    {                                                \
        uint64_t bb = (bitboard);                    \
        while (bb)                                   \
        {                                            \
            int sq = __builtin_ctzll(bb);            \
            hash ^= zobrist_table[(pieceIndex)][sq]; \
            bb &= bb - 1;                            \
        }                                            \
    } while (0)

    // White pieces.
    PROCESS_BITBOARD(board->white_pieces.pawns, WHITE_PAWN);
    PROCESS_BITBOARD(board->white_pieces.knights, WHITE_KNIGHT);
    PROCESS_BITBOARD(board->white_pieces.bishops, WHITE_BISHOP);
    PROCESS_BITBOARD(board->white_pieces.rooks, WHITE_ROOK);
    PROCESS_BITBOARD(board->white_pieces.queens, WHITE_QUEEN);
    PROCESS_BITBOARD(board->white_pieces.king, WHITE_KING);

    // Black pieces.
    PROCESS_BITBOARD(board->black_pieces.pawns, BLACK_PAWN);
    PROCESS_BITBOARD(board->black_pieces.knights, BLACK_KNIGHT);
    PROCESS_BITBOARD(board->black_pieces.bishops, BLACK_BISHOP);
    PROCESS_BITBOARD(board->black_pieces.rooks, BLACK_ROOK);
    PROCESS_BITBOARD(board->black_pieces.queens, BLACK_QUEEN);
    PROCESS_BITBOARD(board->black_pieces.king, BLACK_KING);

    // En passant.
    // If en passant is available (assumed to be a bitboard with a single bit set),
    // XOR the corresponding random value.
    if (board->en_passant)
    {
        int sq = __builtin_ctzll(board->en_passant);
        hash ^= zobrist_en_passant[sq];
    }

    // Castling rights.
    hash ^= zobrist_castling[board->castling_rights];

    // Side to move.
    if (board->side_to_move == BLACK)
        hash ^= zobrist_black_to_move;

    return hash;

#undef PROCESS_BITBOARD
}