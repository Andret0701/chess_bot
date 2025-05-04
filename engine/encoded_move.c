#include "encoded_move.h"

uint8_t get_from_square(uint16_t move)
{
    return move & FROM_MASK;
}

uint8_t get_to_square(uint16_t move)
{
    return (move & TO_MASK) >> 6;
}

uint8_t get_promotion_piece(uint16_t move)
{
    return (move & PROMOTION_MASK) >> 12;
}

bool is_capture_move(uint16_t move)
{
    return (move & CAPTURE_MASK) != 0;
}

bool is_special_move(uint16_t move)
{
    return (move & SPECIAL_MASK) != 0;
}

uint16_t set_from_square(uint16_t move, uint8_t from)
{
    move &= ~FROM_MASK;       // Clear the from square bits
    move |= from & FROM_MASK; // Set the new from square bits
    return move;
}

uint16_t set_to_square(uint16_t move, uint8_t to)
{
    move &= ~TO_MASK;            // Clear the to square bits
    move |= (to << 6) & TO_MASK; // Set the new to square bits
    return move;
}

uint16_t set_promotion_piece(uint16_t move, uint8_t promotion)
{
    move &= ~PROMOTION_MASK;                    // Clear the promotion bits
    move |= (promotion << 12) & PROMOTION_MASK; // Set the new promotion bits
    return move;
}

uint16_t set_capture_move(uint16_t move, bool capture)
{
    if (capture)
        move |= CAPTURE_MASK; // Set the capture flag
    else
        move &= ~CAPTURE_MASK; // Clear the capture flag
    return move;
}

uint16_t set_special_move(uint16_t move, bool special)
{
    if (special)
        move |= SPECIAL_MASK; // Set the special move flag
    else
        move &= ~SPECIAL_MASK; // Clear the special move flag
    return move;
}

uint16_t new_simple_encoded_move(uint8_t from, uint8_t to, bool is_capture)
{
    uint16_t move = 0;
    move = set_from_square(move, from);
    move = set_to_square(move, to);
    move = set_capture_move(move, is_capture);
    return move;
}

uint16_t new_en_passant_encoded_move(uint8_t from, uint8_t to)
{
    uint16_t move = 0;
    move = set_from_square(move, from);
    move = set_to_square(move, to);
    move = set_capture_move(move, true); // Set capture move for en passant
    move = set_special_move(move, true); // Set special move for en passant
    return move;
}

uint16_t new_castling_encoded_move(uint8_t from, uint8_t to)
{
    uint16_t move = 0;
    move = set_from_square(move, from);
    move = set_to_square(move, to);
    move = set_capture_move(move, false); // No capture for castling
    move = set_special_move(move, true);  // Set special move for castling
    return move;
}

uint16_t new_promotion_encoded_move(uint8_t from, uint8_t to, uint8_t promotion, bool is_capture)
{
    uint16_t move = 0;
    move = set_from_square(move, from);
    move = set_to_square(move, to);
    move = set_capture_move(move, is_capture);
    move = set_promotion_piece(move, promotion); // Set the promotion piece
    move = set_special_move(move, true);         // Set special move for promotion
    return move;
}

bool encoded_move_equals(uint16_t move1, uint16_t move2)
{
    // If move is not a special move, clear the promotion bits
    if (!is_special_move(move1))
        move1 = set_promotion_piece(move1, 0);
    if (!is_special_move(move2))
        move2 = set_promotion_piece(move2, 0);

    return move1 == move2;
}

uint16_t board_to_encoded_move(const Board *from, const Board *to)
{
    uint16_t move = 0;

    // Handle castling first - it's a special case where the king moves two squares
    if (from->white_pieces.king == (1ULL << 4))
    {
        if (to->white_pieces.king == (1ULL << 6))
            return encode_move("e1g1", false, true); // White kingside
        if (to->white_pieces.king == (1ULL << 2))
            return encode_move("e1c1", false, true); // White queenside
    }
    if (from->black_pieces.king == (1ULL << 60))
    {
        if (to->black_pieces.king == (1ULL << 62))
            return encode_move("e8g8", false, true); // Black kingside
        if (to->black_pieces.king == (1ULL << 58))
            return encode_move("e8c8", false, true); // Black queenside
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
        return 0; // Something went wrong - shouldn't happen with valid moves

    // Get the square indices
    int source_idx = ctzll(source_squares);
    int dest_idx = ctzll(dest_squares);

    // Check for pawn promotion
    uint8_t promotion = 0;
    bool is_promotion = false;
    if (is_white_move && (source_squares & from->white_pieces.pawns))
    {
        // White pawn reaching 8th rank
        if (dest_idx >= 56 && dest_idx <= 63)
        {
            is_promotion = true;
            if (to->white_pieces.queens & (1ULL << dest_idx))
                promotion = PROMOTION_QUEEN;
            else if (to->white_pieces.rooks & (1ULL << dest_idx))
                promotion = PROMOTION_ROOK;
            else if (to->white_pieces.bishops & (1ULL << dest_idx))
                promotion = PROMOTION_BISHOP;
            else if (to->white_pieces.knights & (1ULL << dest_idx))
                promotion = PROMOTION_KNIGHT;
        }
    }
    else if (!is_white_move && (source_squares & from->black_pieces.pawns))
    {
        // Black pawn reaching 1st rank
        if (dest_idx >= 0 && dest_idx <= 7)
        {
            is_promotion = true;
            if (to->black_pieces.queens & (1ULL << dest_idx))
                promotion = PROMOTION_QUEEN;
            else if (to->black_pieces.rooks & (1ULL << dest_idx))
                promotion = PROMOTION_ROOK;
            else if (to->black_pieces.bishops & (1ULL << dest_idx))
                promotion = PROMOTION_BISHOP;
            else if (to->black_pieces.knights & (1ULL << dest_idx))
                promotion = PROMOTION_KNIGHT;
        }
    }

    bool is_en_passant = false;
    if (is_white_move && (from->black_pieces.pawns != to->black_pieces.pawns) && ((from->black_pieces.pawns & pieces_to_bitboard(&to->white_pieces)) == 0))
        is_en_passant = true;
    else if (!is_white_move && (from->white_pieces.pawns != to->white_pieces.pawns) && ((from->white_pieces.pawns & pieces_to_bitboard(&to->black_pieces)) == 0))
        is_en_passant = true;

    bool is_capture = false;
    if (is_white_move && !pieces_equals(&from->black_pieces, &to->black_pieces))
        is_capture = true;
    else if (!is_white_move && !pieces_equals(&from->white_pieces, &to->white_pieces))
        is_capture = true;
    if (is_en_passant)
        is_capture = true; // En passant is a capture

    move = set_from_square(move, source_idx);
    move = set_to_square(move, dest_idx);
    move = set_promotion_piece(move, promotion);
    move = set_special_move(move, is_en_passant || is_promotion);
    move = set_capture_move(move, is_capture);

    return move;
}

uint16_t encode_move(const char *move, bool is_capture, bool is_special)
{
    // count the length of the move string
    int length = 0;
    while (move[length] != '\0')
        length++;

    if (length < 4)
    {
        printf("Invalid move format\n");
        return 0;
    }

    uint8_t from_file = move[0] - 'a';
    uint8_t from_rank = move[1] - '1';
    uint8_t to_file = move[2] - 'a';
    uint8_t to_rank = move[3] - '1';

    uint16_t encoded_move = 0;
    if (length >= 5)
    {
        uint8_t promotion = 0;
        char promotion_char = move[4];
        switch (promotion_char)
        {
        case 'n':
            promotion = PROMOTION_KNIGHT;
            break;
        case 'b':
            promotion = PROMOTION_BISHOP;
            break;
        case 'r':
            promotion = PROMOTION_ROOK;
            break;
        case 'q':
            promotion = PROMOTION_QUEEN;
            break;
        default:
            printf("Invalid promotion piece\n");
            return 0;
        }
        // Set the promotion bits in the encoded move
        encoded_move |= (promotion << 12) & PROMOTION_MASK;
        is_special = true; // Set special to true if there is a promotion
    }

    encoded_move |= (from_rank * 8 + from_file) & FROM_MASK;  // From square
    encoded_move |= ((to_rank * 8 + to_file) << 6) & TO_MASK; // To square

    if (is_capture)
        encoded_move |= CAPTURE_MASK; // Capture flag

    if (is_special)
        encoded_move |= SPECIAL_MASK; // Special move flag

    return encoded_move;
}

void print_move(uint16_t move)
{
    uint8_t from = get_from_square(move);
    uint8_t to = get_to_square(move);
    uint8_t promotion = get_promotion_piece(move);
    bool capture = is_capture_move(move);
    bool special = is_special_move(move);

    char from_file = 'a' + (from % 8);
    char from_rank = '1' + (from / 8);
    char to_file = 'a' + (to % 8);
    char to_rank = '1' + (to / 8);

    printf("Move: %c%c%c%c, ", from_file, from_rank, to_file, to_rank);
    printf("Promotion: ");
    switch (promotion)
    {
    case PROMOTION_KNIGHT:
        printf("knight, ");
        break;
    case PROMOTION_BISHOP:
        printf("bishop, ");
        break;
    case PROMOTION_ROOK:
        printf("rook, ");
        break;
    case PROMOTION_QUEEN:
        printf("queen, ");
        break;
    default:
        printf("?, ");
        return;
    }
    printf("Capture: %d, ", capture);
    printf("Special: %d", special);
    printf("\n");
}