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

#include "move.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h> // For toupper()
#include "../engine/board_stack.h"
#include "../engine/piece_moves.h"

// Helper: Returns the type of piece at a given square in the board.
// Returns 'K', 'Q', 'R', 'B', 'N', or 'P' if found; otherwise '\0'.
static char get_piece_at(Board board, int square, bool is_white)
{
    uint64_t mask = 1ULL << square;
    if (is_white)
    {
        if (board.white_pieces.king & mask)
            return 'K';
        if (board.white_pieces.queens & mask)
            return 'Q';
        if (board.white_pieces.rooks & mask)
            return 'R';
        if (board.white_pieces.bishops & mask)
            return 'B';
        if (board.white_pieces.knights & mask)
            return 'N';
        if (board.white_pieces.pawns & mask)
            return 'P';
    }
    else
    {
        if (board.black_pieces.king & mask)
            return 'K';
        if (board.black_pieces.queens & mask)
            return 'Q';
        if (board.black_pieces.rooks & mask)
            return 'R';
        if (board.black_pieces.bishops & mask)
            return 'B';
        if (board.black_pieces.knights & mask)
            return 'N';
        if (board.black_pieces.pawns & mask)
            return 'P';
    }
    return '\0';
}

// Helper: Determines if a candidate move corresponds to moving the correct piece
// from the 'from' square to the 'to' square, including promotion if specified.
// - moving_piece: the type (letter) of the piece from the original board.
// - promotion: if nonzero, indicates that the piece should become this type (in lowercase)
//   so we compare after converting to uppercase.
static inline bool candidate_matches_move(Board *board, BoardState candidate, int from, int to, char promotion, bool is_white_move, char moving_piece)
{
    // Ensure that the source square is now empty.
    if (get_piece_at(candidate.board, from, is_white_move) != '\0')
        return false;

    // Determine which piece ended up at the destination.
    char dest_piece = get_piece_at(candidate.board, to, is_white_move);

    // If a promotion is specified, the moved piece must have become that piece.
    if (promotion != '\0')
    {
        char expected = toupper(promotion);
        if (dest_piece != expected)
            return false;
    }
    else
    {
        // Otherwise, the piece type at the destination should match the original moving piece.
        if (dest_piece != moving_piece)
            return false;
    }

    // To avoid castling when the rook moves
    bool moved_king = (moving_piece == 'K' || moving_piece == 'k');
    if (!moved_king)
    {
        if (is_white_move)
            return candidate.board.white_pieces.king == board->white_pieces.king;
        else
            return candidate.board.black_pieces.king == board->black_pieces.king;
    }

    return true;
}

// Improved apply_move function that checks the correct piece is moved to the expected square.
// It first verifies the move string, then generates all candidate moves, and finally selects
// the candidate in which the piece from the source square is removed and appears at the destination.
Board apply_move(Board *board, char *move)
{
    // Validate the move string first.
    if (!move_valid(move))
    {
        // Invalid move string: return original board.
        return *board;
    }

    // Parse the move string into source (from), destination (to), and promotion char (if any).
    int from = (move[0] - 'a') + (move[1] - '1') * 8;
    int to = (move[2] - 'a') + (move[3] - '1') * 8;
    char promotion = (strlen(move) == 5) ? move[4] : '\0';

    // Create bit masks for the source and destination squares.
    uint64_t from_mask = 1ULL << from;

    // Generate all legal moves from the current board state.
    BoardStack *stack = create_board_stack(1000);
    BoardState board_state = board_to_board_state(board);
    generate_moves(&board_state, stack);

    // Determine which side is moving by checking the piece at the source square.
    bool is_white_move = (board_state.white_pieces & from_mask) != 0;

    // Determine the type of piece that is moving from the original board.
    char moving_piece = get_piece_at(board_state.board, from, is_white_move);

    // Iterate through all candidate moves.
    for (uint16_t i = 0; i < stack->count; i++)
    {
        BoardState candidate = stack->boards[i];

        // Check that the candidate move has removed the piece from the source square
        // and placed the correct piece at the destination (including promotion if specified).
        if (candidate_matches_move(board, candidate, from, to, promotion, is_white_move, moving_piece))
        {
            Board result = candidate.board;
            destroy_board_stack(stack);
            return result;
        }
    }

    // No candidate move matched; clean up and return the original board.
    destroy_board_stack(stack);
    return board_state.board;
}
