#pragma once

typedef enum
{
    BOT_SEARCH_TIME,    // Use wtime, btime, winc, binc
    BOT_SEARCH_DEPTH,   // Use depth
    BOT_SEARCH_MOVETIME // Use movetime
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

    // Search movetime
    int movetime; // Time to think in milliseconds
} UCIGoFlags;

void uci_loop();