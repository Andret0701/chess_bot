#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    BOT_SEARCH_TIME,     // Use wtime, btime, winc, binc
    BOT_SEARCH_DEPTH,    // Use depth
    BOT_SEARCH_MOVETIME, // Use movetime
    BOT_SEARCH_NODES,    // Use nodes
    BOT_SEARCH_HEURISTIC // Use heuristic evaluation
} UCIGoOption;

typedef struct
{
    // Search option
    UCIGoOption search_option;

    // Search time
    int wtime; // Time left for white in milliseconds
    int btime; // Time left for black in milliseconds
    int winc;  // Time increment for white in milliseconds
    int binc;  // Time increment for black in milliseconds

    // Search depth
    int depth; // Maximum search depth

    // Search nodes
    uint64_t nodes; // Maximum number of nodes to search

    // Search movetime
    int movetime; // Time to think in milliseconds
} UCIGoFlags;

void uci_loop(bool debug_mode);