#include "count_test.h"
#include "validate_board.h"
#include "../../fen.h"
#include "../piece_moves.h"
typedef struct
{
    const char *fen;
    uint8_t depth;
    uint64_t expected;
} Test;

Test tests[] = {
    // Normal chess board
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1, 20},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 2, 400},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 3, 8902},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4, 197281},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5, 4865609},

    //
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 1, 48},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 2, 2039},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 3, 97862},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 4, 4085603},
    // {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 5, 193690690},

    //
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 1, 14},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 2, 191},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 3, 2812},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 4, 43238},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 5, 674624},

    //
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 1, 6},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 2, 264},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3, 9467},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422333},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5, 15833292},

    //
    {"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 1, 6},
    {"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 2, 264},
    {"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 3, 9467},
    {"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 4, 422333},
    {"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 5, 15833292},

};

uint64_t count_recursive(BoardState *board_state, uint8_t depth, BoardStack *stack)
{
    validate_board(&board_state->board);
    if (depth == 0)
        return 1;

    uint32_t base = stack->count;
    generate_moves(&board_state->board, stack);

    if (stack->count == base)
        return 0;

    uint64_t total = 0;
    for (uint16_t i = base; i < stack->count; i++)
    {
        total += count_recursive(&stack->boards[i], depth - 1, stack);
    }

    stack->count = base;
    return total;
}

void run_count_tests()
{
    BoardStack *stack = create_board_stack(65535);
    for (uint8_t i = 0; i < sizeof(tests) / sizeof(Test); i++)
    {
        stack->count = 0;
        Board board = fen_to_board(tests[i].fen);
        BoardState board_state = board_to_board_state(&board);

        uint64_t result = count_recursive(&board_state, tests[i].depth, stack);
        if (result != tests[i].expected)
            printf(":( Test %u failed. Expected %llu, got %llu. Off by %lld\n", i, tests[i].expected, result, ((int64_t)result) - ((int64_t)tests[i].expected));
        else
            printf(":) Test %u passed. Expected %llu, got %llu\n", i, tests[i].expected, result);

        board = flip_board(&board);
        board_state = board_to_board_state(&board);
        result = count_recursive(&board_state, tests[i].depth, stack);
        if (result != tests[i].expected)
            printf(":( Flipped %u failed. Expected %llu, got %llu. Off by %lld\n", i, tests[i].expected, result, ((int64_t)result) - ((int64_t)tests[i].expected));
        else
            printf(":) Flipped %u passed. Expected %llu, got %llu\n", i, tests[i].expected, result);
    }
    destroy_board_stack(stack);
}
