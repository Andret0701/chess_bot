#include "move.h"

char *board_to_move(Board* from, Board* to)
{
    static char move[6];
    move[5] = '\0'; // Ensure the string is null-terminated.

    // Handle castling (simplified for clarity)
    if (from->white_pieces.king == (1ULL << 4) && to->white_pieces.king == (1ULL << 6)) // White king-side castling
    {
        strcpy(move, "e1g1\0");
        return move;
    }
    if (from->white_pieces.king == (1ULL << 4) && to->white_pieces.king == (1ULL << 2)) // White queen-side castling
    {
        strcpy(move, "e1c1\0");
        return move;
    }
    if (from->black_pieces.king == (1ULL << 60) && to->black_pieces.king == (1ULL << 62)) // Black king-side castling
    {
        strcpy(move, "e8g8\0");
        return move;
    }
    if (from->black_pieces.king == (1ULL << 60) && to->black_pieces.king == (1ULL << 58)) // Black queen-side castling
    {
        strcpy(move, "e8c8\0");
        return move;
    }

    uint64_t from_white_pieces = from->white_pieces.pawns | from->white_pieces.knights | from->white_pieces.bishops | from->white_pieces.rooks | from->white_pieces.queens | from->white_pieces.king;
    uint64_t from_black_pieces = from->black_pieces.pawns | from->black_pieces.knights | from->black_pieces.bishops | from->black_pieces.rooks | from->black_pieces.queens | from->black_pieces.king;

    uint64_t from_occupied = from_white_pieces | from_black_pieces;

    uint64_t to_white_pieces = to->white_pieces.pawns | to->white_pieces.knights | to->white_pieces.bishops | to->white_pieces.rooks | to->white_pieces.queens | to->white_pieces.king;
    uint64_t to_black_pieces = to->black_pieces.pawns | to->black_pieces.knights | to->black_pieces.bishops | to->black_pieces.rooks | to->black_pieces.queens | to->black_pieces.king;

    uint64_t to_occupied = to_white_pieces | to_black_pieces;


    uint64_t moved_piece = from_occupied ^ to_occupied;
    uint64_t source_square = from_occupied & ~to_occupied;

    bool is_white_move = (moved_piece & from_white_pieces) != 0;
    uint64_t destination_square = is_white_move ? to_white_pieces & ~from_white_pieces : to_black_pieces & ~from_black_pieces;
    char promotion = '\0';

    // Calculate source and destination squares
    int source_square_index = ctzll(source_square);
    int destination_square_index = ctzll(destination_square);

    // Determine if a pawn promotion occurred
    if ((source_square & from->white_pieces.pawns) && (destination_square_index >= 56 && destination_square_index <= 63))
    {
        if (to->white_pieces.queens & (1ULL << destination_square_index))
            promotion = 'q';
        else if (to->white_pieces.rooks & (1ULL << destination_square_index))
            promotion = 'r';
        else if (to->white_pieces.bishops & (1ULL << destination_square_index))
            promotion = 'b';
        else if (to->white_pieces.knights & (1ULL << destination_square_index))
            promotion = 'n';
    }
    else if ((source_square & from->black_pieces.pawns) && (destination_square_index >= 0 && destination_square_index <= 7))
    {
        if (to->black_pieces.queens & (1ULL << destination_square_index))
            promotion = 'q';
        else if (to->black_pieces.rooks & (1ULL << destination_square_index))
            promotion = 'r';
        else if (to->black_pieces.bishops & (1ULL << destination_square_index))
            promotion = 'b';
        else if (to->black_pieces.knights & (1ULL << destination_square_index))
            promotion = 'n';
    }

    // Convert indices to board notation (e.g., 0 -> 'a1', 63 -> 'h8')
    move[0] = 'a' + (source_square_index % 8);
    move[1] = '1' + (source_square_index / 8);
    move[2] = 'a' + (destination_square_index % 8);
    move[3] = '1' + (destination_square_index / 8);

    // Add promotion piece if needed.
    if (promotion != '\0')
        move[4] = promotion;
    else
        move[4] = '\0';

    return move;
}