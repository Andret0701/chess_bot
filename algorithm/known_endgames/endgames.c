#include "endgames.h"

#include "./pyrrhic/tbprobe.h"

#include <stdio.h>
#include <stdlib.h>

void init_endgames()
{
    bool initialized = tb_init("./3-4-5");
    if (!initialized)
    {
        fprintf(stderr, "Failed to initialize endgames\n");
        exit(EXIT_FAILURE);
    }
}

void free_endgames()
{
    tb_free();
}

bool is_known_endgame(BoardState *board_state)
{
    uint8_t piece_count = popcountll(board_state->occupied);
    if (piece_count > 5)
        return false;

    return true;
}

BoardScore score_endgame(BoardState *board_state)
{
    if (!is_known_endgame(board_state))
    {
        fprintf(stderr, "Not an endgame\n");
        exit(EXIT_FAILURE);
    }

    unsigned ep = board_state->board.en_passant;
    unsigned ep_square = (ep != 0) ? __builtin_ctz(ep) : 0;

    // unsigned tb_probe_wdl(
    //     uint64_t white, uint64_t black,
    //     uint64_t kings, uint64_t queens,
    //     uint64_t rooks, uint64_t bishops,
    //     uint64_t knights, uint64_t pawns,
    //     unsigned ep, bool turn)
    uint32_t result = tb_probe_wdl(
        board_state->white_pieces, board_state->black_pieces,
        board_state->board.white_pieces.king | board_state->board.black_pieces.king,
        board_state->board.white_pieces.queens | board_state->board.black_pieces.queens,
        board_state->board.white_pieces.rooks | board_state->board.black_pieces.rooks,
        board_state->board.white_pieces.bishops | board_state->board.black_pieces.bishops,
        board_state->board.white_pieces.knights | board_state->board.black_pieces.knights,
        board_state->board.white_pieces.pawns | board_state->board.black_pieces.pawns,
        ep_square, board_state->board.side_to_move == WHITE);

    if (result == TB_RESULT_FAILED)
    {
        fprintf(stderr, "Failed to probe endgame\n");
        exit(EXIT_FAILURE);
    }

    int wdl = TB_RESULT_WDL(result);

    Result game_result = DRAW;
    uint8_t depth = TB_RESULT_DTZ(result);
    if (wdl == TB_WIN)
        game_result = board_state->board.side_to_move == WHITE ? WHITE_WON : BLACK_WON;
    if (wdl == TB_LOSS)
        game_result = board_state->board.side_to_move == WHITE ? BLACK_WON : WHITE_WON;

    return (BoardScore){0, game_result, depth};
}