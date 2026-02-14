#include "zobrist_hash.h"
#include <stdlib.h>
#include <time.h>

#define NUM_PIECE_TYPES 12
#define NUM_SQUARES 64
enum
{
    WHITE_PAWN = 0,
    WHITE_KNIGHT = 1,
    WHITE_BISHOP = 2,
    WHITE_ROOK = 3,
    WHITE_QUEEN = 4,
    WHITE_KING = 5,
    BLACK_PAWN = 6,
    BLACK_KNIGHT = 7,
    BLACK_BISHOP = 8,
    BLACK_ROOK = 9,
    BLACK_QUEEN = 10,
    BLACK_KING = 11
};

uint64_t zobrist_table[NUM_PIECE_TYPES][NUM_SQUARES];
uint64_t zobrist_castling[16];
uint64_t zobrist_en_passant[NUM_SQUARES];
uint64_t zobrist_black_to_move;

static uint64_t rng_state;

void seed_rng(uint64_t seed)
{
    rng_state = (seed != 0) ? seed : 88172645463325252ULL;
}

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
    srand(time(NULL));
    seed_rng((uint64_t)time(NULL) + rand());

    for (int piece = 0; piece < NUM_PIECE_TYPES; piece++)
    {
        for (int sq = 0; sq < NUM_SQUARES; sq++)
        {
            zobrist_table[piece][sq] = random_64bit();
        }
    }

    for (int i = 0; i < 16; i++)
    {
        zobrist_castling[i] = random_64bit();
    }

    for (int sq = 0; sq < NUM_SQUARES; sq++)
    {
        zobrist_en_passant[sq] = random_64bit();
    }

    zobrist_black_to_move = random_64bit();
}

uint64_t hash_board(Board *board)
{
    uint64_t hash = 0;

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

    PROCESS_BITBOARD(board->white_pieces.pawns, WHITE_PAWN);
    PROCESS_BITBOARD(board->white_pieces.knights, WHITE_KNIGHT);
    PROCESS_BITBOARD(board->white_pieces.bishops, WHITE_BISHOP);
    PROCESS_BITBOARD(board->white_pieces.rooks, WHITE_ROOK);
    PROCESS_BITBOARD(board->white_pieces.queens, WHITE_QUEEN);
    PROCESS_BITBOARD(board->white_pieces.king, WHITE_KING);

    PROCESS_BITBOARD(board->black_pieces.pawns, BLACK_PAWN);
    PROCESS_BITBOARD(board->black_pieces.knights, BLACK_KNIGHT);
    PROCESS_BITBOARD(board->black_pieces.bishops, BLACK_BISHOP);
    PROCESS_BITBOARD(board->black_pieces.rooks, BLACK_ROOK);
    PROCESS_BITBOARD(board->black_pieces.queens, BLACK_QUEEN);
    PROCESS_BITBOARD(board->black_pieces.king, BLACK_KING);

    if (board->en_passant)
    {
        int ep_sq = __builtin_ctzll(board->en_passant);
        int ep_file = ep_sq & 7;

        bool ep_is_capturable = false;

        if (board->side_to_move == WHITE)
        {
            uint64_t pawns = board->white_pieces.pawns;
            uint64_t from_left = (ep_file > 0) ? (1ULL << (ep_sq - 9)) : 0ULL;
            uint64_t from_right = (ep_file < 7) ? (1ULL << (ep_sq - 7)) : 0ULL;
            ep_is_capturable = (pawns & (from_left | from_right)) != 0;
        }
        else
        {
            uint64_t pawns = board->black_pieces.pawns;
            uint64_t from_left = (ep_file > 0) ? (1ULL << (ep_sq + 7)) : 0ULL;
            uint64_t from_right = (ep_file < 7) ? (1ULL << (ep_sq + 9)) : 0ULL;
            ep_is_capturable = (pawns & (from_left | from_right)) != 0;
        }

        if (ep_is_capturable)
            hash ^= zobrist_en_passant[ep_sq];
    }

    hash ^= zobrist_castling[board->castling_rights];

    if (board->side_to_move == BLACK)
        hash ^= zobrist_black_to_move;

    return hash;

#undef PROCESS_BITBOARD
}
