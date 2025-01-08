#include "endgames.h"
#include "../game_history.h"
#include "./pyrrhic/tbprobe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables to track tablebase state
static bool tablebases_initialized = false;
static char tablebase_path[4096] = "./3-4-5"; // Default path

void set_tablebase_path(const char *path)
{
    if (path && strlen(path) < sizeof(tablebase_path))
    {
        strncpy(tablebase_path, path, sizeof(tablebase_path) - 1);
        tablebase_path[sizeof(tablebase_path) - 1] = '\0';
    }
    else
    {
        fprintf(stderr, "Invalid tablebase path provided\n");
        exit(EXIT_FAILURE);
    }
}

bool init_endgames()
{
    if (tablebases_initialized)
    {
        return true; // Already initialized
    }

    bool initialized = tb_init(tablebase_path);
    if (!initialized)
    {
        fprintf(stderr, "Failed to initialize endgames - could not find tablebase files in %s\n",
                tablebase_path);
        fprintf(stderr, "Make sure Syzygy format (.rtbw, .rtbm, .rtbz) files are present\n");
        return false;
    }

    // fprintf(stderr, "Successfully loaded tablebases:\n");
    // fprintf(stderr, "Maximum pieces: %d\n", TB_LARGEST);
    // fprintf(stderr, "WDL tables: %d\n", TB_NUM_WDL);
    // fprintf(stderr, "DTZ tables: %d\n", TB_NUM_DTZ);

    tablebases_initialized = true;
    return true;
}

void free_endgames()
{
    if (tablebases_initialized)
    {
        tb_free();
        tablebases_initialized = false;
    }
}

bool is_known_endgame(BoardState *board_state)
{
    if (!tablebases_initialized)
    {
        fprintf(stderr, "Endgame tables not initialized\n");
        return false;
    }

    uint8_t piece_count = popcountll(board_state->occupied);

    // Check if position has too many pieces for our tablebases
    if (piece_count > TB_LARGEST)
    {
        return false;
    }

    // If any player has castling rights, it's not an endgame
    if (board_state->board.castling_rights != 0)
        return false;

    return true;
}
BoardScore score_endgame(BoardState *board_state)
{
    if (!tablebases_initialized)
    {
        fprintf(stderr, "Endgame tables not initialized\n");
        exit(EXIT_FAILURE);
    }

    if (!is_known_endgame(board_state))
    {
        fprintf(stderr, "Position is not a known endgame\n");
        exit(EXIT_FAILURE);
    }

    // Handle en passant square
    unsigned ep = board_state->board.en_passant;
    unsigned ep_square = (ep != 0) ? __builtin_ctz(ep) : 0;

    // Get current 50-move counter
    uint8_t rule50 = get_50_move_count();

    unsigned results[TB_MAX_MOVES];

    unsigned result = tb_probe_root(
        board_state->white_pieces, board_state->black_pieces,
        board_state->board.white_pieces.king | board_state->board.black_pieces.king,
        board_state->board.white_pieces.queens | board_state->board.black_pieces.queens,
        board_state->board.white_pieces.rooks | board_state->board.black_pieces.rooks,
        board_state->board.white_pieces.bishops | board_state->board.black_pieces.bishops,
        board_state->board.white_pieces.knights | board_state->board.black_pieces.knights,
        board_state->board.white_pieces.pawns | board_state->board.black_pieces.pawns,
        rule50, ep_square,
        board_state->board.side_to_move == WHITE, results);

    if (result == TB_RESULT_FAILED)
    {
        fprintf(stderr, "Failed to probe tablebase\n");
        exit(EXIT_FAILURE);
    }

    if (result == TB_RESULT_STALEMATE)
        return (BoardScore){0, DRAW, 0};

    if (result == TB_RESULT_CHECKMATE)
        return (BoardScore){0, board_state->board.side_to_move == WHITE ? BLACK_WON : WHITE_WON, 0};

    // Get WDL and DTZ from the result
    int wdl = TB_GET_WDL(result);
    int dtz = TB_GET_DTZ(result);

    Result game_result;
    if (wdl == TB_WIN) // TB WIN
        game_result = board_state->board.side_to_move == WHITE ? WHITE_WON : BLACK_WON;
    else if (wdl == TB_CURSED_WIN) // TB CURSED WIN - drawn by 50-move rule
        game_result = DRAW;
    else if (wdl == TB_DRAW) // TB DRAW
        game_result = DRAW;
    else if (wdl == TB_BLESSED_LOSS) // TB BLESSED LOSS - drawn by 50-move rule
        game_result = DRAW;
    else // TB LOSS
        game_result = board_state->board.side_to_move == WHITE ? BLACK_WON : WHITE_WON;

    return (BoardScore){0, game_result, dtz};
}