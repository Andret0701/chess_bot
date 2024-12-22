#include "move.h"
#include <string.h>
#include <stdbool.h>
#include "engine/board_stack.h"
#include "engine/piece_moves.h"

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

bool move_valid(char *move)
{
    if (strlen(move) < 4 || strlen(move) > 5)
        return false;

    if (move[0] < 'a' || move[0] > 'h')
        return false;

    if (move[1] < '1' || move[1] > '8')
        return false;

    if (move[2] < 'a' || move[2] > 'h')
        return false;

    if (move[3] < '1' || move[3] > '8')
        return false;

    if (strlen(move) == 5)
    {
        if (move[4] != 'q' && move[4] != 'r' && move[4] != 'b' && move[4] != 'n')
            return false;
    }

    return true;
}

bool can_move(Board* board, char* move)
{
    if (!move_valid(move))
        return false;

    Board new_board = apply_move(board, move);
    return !board_equals(board, &new_board);    

}

Board apply_move(Board* board, char *move)
{
    // Calculate the 'from' and 'to' positions from the move string
    int from = (move[0] - 'a') + (move[1] - '1') * 8;
    int to = (move[2] - 'a') + (move[3] - '1') * 8;

    // Check if there's a promotion character
    char promotion = (strlen(move) == 5) ? move[4] : '\0';

    uint64_t from_mask = 1ULL << from;
    uint64_t to_mask = 1ULL << to;

    // Generate all possible moves and store them in a stack
    BoardStack *stack = create_board_stack(1000);
    BoardState board_state = board_to_board_state(board);
    generate_moves(&board_state, stack);

    // Determine if it's white's move or black's move based on the piece at 'from'
    int is_white_move = (board_state.white_pieces & from_mask) != 0;

    for (uint16_t i = 0; i < stack->count; i++)
    {
        BoardState candidate = stack->boards[i];

        // Check if the move corresponds to the correct piece being moved
        if (is_white_move)
        {
            if ((!(candidate.white_pieces & from_mask)) && (candidate.white_pieces & to_mask))
            {
                // Handle promotion
                if (promotion != '\0')
                {
                    if ((promotion == 'q' && (candidate.board.white_pieces.queens & to_mask)) ||
                        (promotion == 'r' && (candidate.board.white_pieces.rooks & to_mask)) ||
                        (promotion == 'b' && (candidate.board.white_pieces.bishops & to_mask)) ||
                        (promotion == 'n' && (candidate.board.white_pieces.knights & to_mask)))
                    {
                        destroy_board_stack(stack);
                        return candidate.board;
                    }
                }
                else
                {
                    destroy_board_stack(stack);
                    return candidate.board;
                }
            }
        }
        else // Black's move
        {
            if ((!(candidate.black_pieces & from_mask)) && (candidate.black_pieces & to_mask))
            {
                // Handle promotion
                if (promotion != '\0')
                {
                    if ((promotion == 'q' && (candidate.board.black_pieces.queens & to_mask)) ||
                        (promotion == 'r' && (candidate.board.black_pieces.rooks & to_mask)) ||
                        (promotion == 'b' && (candidate.board.black_pieces.bishops & to_mask)) ||
                        (promotion == 'n' && (candidate.board.black_pieces.knights & to_mask)))
                    {
                        destroy_board_stack(stack);
                        return candidate.board;
                    }
                }
                else
                {
                    destroy_board_stack(stack);
                    return candidate.board;
                }
            }
        }
    }

    // If no matching move was found, return the original board
    destroy_board_stack(stack);
    return board_state.board;
}