#include "endgames.h"
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

    // Additional validation could be added here
    // For example, checking specific piece configurations

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

    // Probe the tablebase
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
        // Provide more detailed error information
        fprintf(stderr, "Failed to probe endgame tablebase\n");
        fprintf(stderr, "Piece count: %d\n", popcountll(board_state->occupied));
        fprintf(stderr, "EP square: %u\n", ep_square);
        fprintf(stderr, "Side to move: %s\n", board_state->board.side_to_move == WHITE ? "white" : "black");
        exit(EXIT_FAILURE);
    }

    // First get WDL (Win/Draw/Loss)
    int wdl = TB_GET_WDL(result);

    uint8_t rule50 = 0; // Implement 50-move rule

    // Now probe for DTZ specifically using tb_probe_root
    unsigned results[TB_MAX_MOVES];
    result = tb_probe_root(
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
        fprintf(stderr, "Failed to probe endgame tablebase for DTZ\n");
        exit(EXIT_FAILURE);
    }

    Result game_result = DRAW;
    if (wdl == TB_WIN)
        game_result = board_state->board.side_to_move == WHITE ? WHITE_WON : BLACK_WON;
    else if (wdl == TB_LOSS)
        game_result = board_state->board.side_to_move == WHITE ? BLACK_WON : WHITE_WON;

    uint8_t dtz = TB_GET_DTZ(result);
    return (BoardScore){0, game_result, dtz};
}

// Helper function to print board state for debugging
void print_board_state(BoardState *board_state)
{
    fprintf(stderr, "Board State:\n");
    fprintf(stderr, "White pieces: 0x%llx\n", (unsigned long long)board_state->white_pieces);
    fprintf(stderr, "Black pieces: 0x%llx\n", (unsigned long long)board_state->black_pieces);
    fprintf(stderr, "Occupied squares: 0x%llx\n", (unsigned long long)board_state->occupied);
    // Add more debug information as needed
}