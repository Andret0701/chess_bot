#include "move.h"
#include <string.h>
#include <stdbool.h>
#include "../engine/board_stack.h"
#include "../engine/piece_moves.h"

char *board_to_move(Board *from, Board *to)
{
    static char move[6];
    move[5] = '\0'; // Ensure null termination

    // Handle castling first - it's a special case where the king moves two squares
    if (from->white_pieces.king == (1ULL << 4))
    {
        if (to->white_pieces.king == (1ULL << 6))
        {
            return strcpy(move, "e1g1"); // White kingside
        }
        if (to->white_pieces.king == (1ULL << 2))
        {
            return strcpy(move, "e1c1"); // White queenside
        }
    }
    if (from->black_pieces.king == (1ULL << 60))
    {
        if (to->black_pieces.king == (1ULL << 62))
        {
            return strcpy(move, "e8g8"); // Black kingside
        }
        if (to->black_pieces.king == (1ULL << 58))
        {
            return strcpy(move, "e8c8"); // Black queenside
        }
    }

    // Get all piece positions for each side
    uint64_t from_white = pieces_to_bitboard(&from->white_pieces);
    uint64_t from_black = pieces_to_bitboard(&from->black_pieces);
    uint64_t to_white = pieces_to_bitboard(&to->white_pieces);
    uint64_t to_black = pieces_to_bitboard(&to->black_pieces);

    // Determine which side moved and get their piece bitboards
    bool is_white_move = (from->side_to_move == WHITE);
    uint64_t moving_side_from = is_white_move ? from_white : from_black;
    uint64_t moving_side_to = is_white_move ? to_white : to_black;

    // Find source square (must be in 'from' position but not in 'to' position)
    uint64_t source_squares = moving_side_from & ~moving_side_to;

    // Find destination square (must be in 'to' position but not in 'from' position)
    uint64_t dest_squares = moving_side_to & ~moving_side_from;

    // Validate we found exactly one source and one destination
    if (popcountll(source_squares) != 1 || popcountll(dest_squares) != 1)
    {
        // Something went wrong - shouldn't happen with valid moves
        move[0] = '\0';
        return move;
    }

    // Get the square indices
    int source_idx = ctzll(source_squares);
    int dest_idx = ctzll(dest_squares);

    // Convert to algebraic notation
    move[0] = 'a' + (source_idx % 8);
    move[1] = '1' + (source_idx / 8);
    move[2] = 'a' + (dest_idx % 8);
    move[3] = '1' + (dest_idx / 8);
    move[4] = '\0';

    // Check for pawn promotion
    if (is_white_move && (source_squares & from->white_pieces.pawns))
    {
        // White pawn reaching 8th rank
        if (dest_idx >= 56 && dest_idx <= 63)
        {
            if (to->white_pieces.queens & (1ULL << dest_idx))
                move[4] = 'q';
            else if (to->white_pieces.rooks & (1ULL << dest_idx))
                move[4] = 'r';
            else if (to->white_pieces.bishops & (1ULL << dest_idx))
                move[4] = 'b';
            else if (to->white_pieces.knights & (1ULL << dest_idx))
                move[4] = 'n';
        }
    }
    else if (!is_white_move && (source_squares & from->black_pieces.pawns))
    {
        // Black pawn reaching 1st rank
        if (dest_idx >= 0 && dest_idx <= 7)
        {
            if (to->black_pieces.queens & (1ULL << dest_idx))
                move[4] = 'q';
            else if (to->black_pieces.rooks & (1ULL << dest_idx))
                move[4] = 'r';
            else if (to->black_pieces.bishops & (1ULL << dest_idx))
                move[4] = 'b';
            else if (to->black_pieces.knights & (1ULL << dest_idx))
                move[4] = 'n';
        }
    }

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

bool can_move(Board *board, char *move)
{
    if (!move_valid(move))
        return false;

    Board new_board = apply_move(board, move);
    return !board_equals(board, &new_board);
}

Board apply_move(Board *board, char *move)
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