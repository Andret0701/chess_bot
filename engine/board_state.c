#include "board_state.h"
#include "piece_moves.h"
#include "board_stack.h"

BoardState board_to_board_state(Board *board)
{
    BoardState board_state = {0};
    copy_board(board, &board_state.board);
    init_board(&board_state);
    return board_state;
}

Result get_game_result(BoardState *board_state)
{
    BoardStack *stack = create_board_stack(BOARD_STACK_SIZE);
    generate_moves(board_state, stack);
    bool is_finished = stack->count == 0;
    destroy_board_stack(stack);

    if (is_finished)
    {
        if (board_state->board.side_to_move == WHITE)
            return board_state->white_check ? LOST : STALEMATE;
        else
            return board_state->black_check ? LOST : STALEMATE;
    }
    return UNKNOWN;
}

BoardState do_move(const BoardState *board_state, uint16_t move)
{
    BoardState new_board_state;
    copy_board(&board_state->board, &new_board_state.board);
    new_board_state.white_pieces = board_state->white_pieces;
    new_board_state.black_pieces = board_state->black_pieces;
    new_board_state.occupied = board_state->occupied;
    new_board_state.white_attack = board_state->white_attack;
    new_board_state.black_attack = board_state->black_attack;
    new_board_state.has_castled = board_state->has_castled;

    uint8_t from_square = get_from_square(move);
    uint8_t to_square = get_to_square(move);
    uint8_t from_x = from_square % 8;
    uint8_t from_y = from_square / 8;
    uint8_t to_x = to_square % 8;
    uint8_t to_y = to_square / 8;
    uint64_t from_bitboard = 1ULL << from_square;
    uint64_t to_bitboard = 1ULL << to_square;

    if (new_board_state.board.side_to_move == WHITE)
    {
        Piece piece = get_white_piece(&new_board_state.board, from_bitboard);
        switch (piece)
        {
        case PAWN:
            if (to_y == 7) // Promotion
            {
                new_board_state.board.white_pieces.pawns &= ~from_bitboard;
                switch (get_promotion_piece(move))
                {
                case PROMOTION_QUEEN:
                    new_board_state.board.white_pieces.queens |= to_bitboard;
                    break;
                case PROMOTION_ROOK:
                    new_board_state.board.white_pieces.rooks |= to_bitboard;
                    break;
                case PROMOTION_BISHOP:
                    new_board_state.board.white_pieces.bishops |= to_bitboard;
                    break;
                case PROMOTION_KNIGHT:
                    new_board_state.board.white_pieces.knights |= to_bitboard;
                    break;
                }

                new_board_state.white_pieces = move_bit(new_board_state.white_pieces, from_bitboard, to_bitboard);
                new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
                new_board_state.board.en_passant = 0;
                if (is_capture_move(move))
                    remove_black_piece(&new_board_state, to_x, to_y);
            }
            else if (from_y == 1 && to_y == 3) // Double move
            {
                new_board_state.board.white_pieces.pawns = move_bit(new_board_state.board.white_pieces.pawns, from_bitboard, to_bitboard);
                new_board_state.white_pieces = move_bit(new_board_state.white_pieces, from_bitboard, to_bitboard);
                new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
                new_board_state.board.en_passant = position_to_bitboard(to_x, to_y - 1);
            }
            else if (from_x != to_x && !is_black_piece(&new_board_state, to_x, to_y)) // En passant
            {
                new_board_state.board.white_pieces.pawns = move_bit(new_board_state.board.white_pieces.pawns, from_bitboard, to_bitboard);
                new_board_state.white_pieces = move_bit(new_board_state.white_pieces, from_bitboard, to_bitboard);
                new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
                // Remove captured pawn (below the destination square)
                uint64_t capture_bitboard = new_board_state.board.en_passant >> 8;
                new_board_state.board.black_pieces.pawns &= ~capture_bitboard;
                new_board_state.black_pieces &= ~capture_bitboard;
                new_board_state.occupied &= ~capture_bitboard;
                new_board_state.board.en_passant = 0;
            }
            else // Regular pawn move
            {
                new_board_state.board.white_pieces.pawns = move_bit(new_board_state.board.white_pieces.pawns, from_bitboard, to_bitboard);
                new_board_state.white_pieces = move_bit(new_board_state.white_pieces, from_bitboard, to_bitboard);
                new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
                new_board_state.board.en_passant = 0;
                if (is_capture_move(move))
                    remove_black_piece(&new_board_state, to_x, to_y);
            }
            break;
        case KNIGHT:
            new_board_state.board.white_pieces.knights = move_bit(new_board_state.board.white_pieces.knights, from_bitboard, to_bitboard);
            new_board_state.white_pieces = move_bit(new_board_state.white_pieces, from_bitboard, to_bitboard);
            new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
            new_board_state.board.en_passant = 0;
            if (is_capture_move(move))
                remove_black_piece(&new_board_state, to_x, to_y);
            break;
        case BISHOP:
            new_board_state.board.white_pieces.bishops = move_bit(new_board_state.board.white_pieces.bishops, from_bitboard, to_bitboard);
            new_board_state.white_pieces = move_bit(new_board_state.white_pieces, from_bitboard, to_bitboard);
            new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
            new_board_state.board.en_passant = 0;
            if (is_capture_move(move))
                remove_black_piece(&new_board_state, to_x, to_y);
            break;
        case ROOK:
            new_board_state.board.white_pieces.rooks = move_bit(new_board_state.board.white_pieces.rooks, from_bitboard, to_bitboard);
            new_board_state.white_pieces = move_bit(new_board_state.white_pieces, from_bitboard, to_bitboard);
            new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
            new_board_state.board.en_passant = 0;
            if (is_capture_move(move))
                remove_black_piece(&new_board_state, to_x, to_y);

            if (from_x == 0 && from_y == 0) // Queenside rook
                new_board_state.board.castling_rights &= ~WHITE_QUEENSIDE_CASTLE;

            if (from_x == 7 && from_y == 0) // Kingside rook
                new_board_state.board.castling_rights &= ~WHITE_KINGSIDE_CASTLE;
            break;
        case QUEEN:
            new_board_state.board.white_pieces.queens = move_bit(new_board_state.board.white_pieces.queens, from_bitboard, to_bitboard);
            new_board_state.white_pieces = move_bit(new_board_state.white_pieces, from_bitboard, to_bitboard);
            new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
            new_board_state.board.en_passant = 0;
            if (is_capture_move(move))
                remove_black_piece(&new_board_state, to_x, to_y);
            break;
        case KING:
            // Remove white castling rights when king moves
            new_board_state.board.castling_rights &= ~(WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE);

            if (from_x == 4 && from_y == 0 && to_x == 6 && to_y == 0) // Kingside castle
            {
                // Move king
                new_board_state.board.white_pieces.king = move_bit(new_board_state.board.white_pieces.king, position_to_bitboard(4, 0), position_to_bitboard(6, 0));
                // Move rook
                new_board_state.board.white_pieces.rooks = move_bit(new_board_state.board.white_pieces.rooks, position_to_bitboard(7, 0), position_to_bitboard(5, 0));
                // Update aggregated bitboards
                new_board_state.white_pieces = (new_board_state.board.white_pieces.pawns |
                                                new_board_state.board.white_pieces.knights |
                                                new_board_state.board.white_pieces.bishops |
                                                new_board_state.board.white_pieces.rooks |
                                                new_board_state.board.white_pieces.queens |
                                                new_board_state.board.white_pieces.king);
                new_board_state.occupied = new_board_state.white_pieces | new_board_state.black_pieces;
                new_board_state.has_castled |= WHITE_KINGSIDE_CASTLE;
            }
            else if (from_x == 4 && from_y == 0 && to_x == 2 && to_y == 0) // Queenside castle
            {
                // Move king
                new_board_state.board.white_pieces.king = move_bit(new_board_state.board.white_pieces.king, position_to_bitboard(4, 0), position_to_bitboard(2, 0));
                // Move rook
                new_board_state.board.white_pieces.rooks = move_bit(new_board_state.board.white_pieces.rooks, position_to_bitboard(0, 0), position_to_bitboard(3, 0));
                // Update aggregated bitboards
                new_board_state.white_pieces = (new_board_state.board.white_pieces.pawns |
                                                new_board_state.board.white_pieces.knights |
                                                new_board_state.board.white_pieces.bishops |
                                                new_board_state.board.white_pieces.rooks |
                                                new_board_state.board.white_pieces.queens |
                                                new_board_state.board.white_pieces.king);
                new_board_state.occupied = new_board_state.white_pieces | new_board_state.black_pieces;
                new_board_state.has_castled |= WHITE_QUEENSIDE_CASTLE;
            }
            else // Regular king move
            {
                new_board_state.board.white_pieces.king = move_bit(new_board_state.board.white_pieces.king, from_bitboard, to_bitboard);
                new_board_state.white_pieces = move_bit(new_board_state.white_pieces, from_bitboard, to_bitboard);
                new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
                if (is_capture_move(move))
                    remove_black_piece(&new_board_state, to_x, to_y);
            }
            new_board_state.board.castling_rights &= ~(WHITE_KINGSIDE_CASTLE | WHITE_QUEENSIDE_CASTLE);
            new_board_state.board.en_passant = 0;
            break;
        }

        if (is_capture_move(move))
        {
            // check if rooks are not on start position. remove castling rights
            if (to_x == 0 && to_y == 7) // White queenside rook captured
                new_board_state.board.castling_rights &= ~BLACK_QUEENSIDE_CASTLE;
            if (to_x == 7 && to_y == 7) // White kingside rook captured
                new_board_state.board.castling_rights &= ~BLACK_KINGSIDE_CASTLE;
        }

        new_board_state.board.side_to_move = BLACK;
        // this is bad
        generate_black_attacks(board_state, &new_board_state);
        new_board_state.white_check = new_board_state.board.white_pieces.king & new_board_state.black_attack;
        if (new_board_state.white_check)
            return new_board_state;
        generate_white_attacks(board_state, &new_board_state);
        new_board_state.black_check = new_board_state.board.black_pieces.king & new_board_state.white_attack;
        ///
    }
    else // BLACK's turn
    {
        Piece piece = get_black_piece(&new_board_state.board, from_bitboard);
        switch (piece)
        {
        case PAWN:
            if (to_y == 0) // Promotion
            {
                new_board_state.board.black_pieces.pawns &= ~from_bitboard;
                switch (get_promotion_piece(move))
                {
                case PROMOTION_QUEEN:
                    new_board_state.board.black_pieces.queens |= to_bitboard;
                    break;
                case PROMOTION_ROOK:
                    new_board_state.board.black_pieces.rooks |= to_bitboard;
                    break;
                case PROMOTION_BISHOP:
                    new_board_state.board.black_pieces.bishops |= to_bitboard;
                    break;
                case PROMOTION_KNIGHT:
                    new_board_state.board.black_pieces.knights |= to_bitboard;
                    break;
                }
                // Update bitboards
                new_board_state.black_pieces = move_bit(new_board_state.black_pieces, from_bitboard, to_bitboard);
                new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
                new_board_state.board.en_passant = 0;
                if (is_capture_move(move))
                    remove_white_piece(&new_board_state, to_x, to_y);
            }
            else if (from_y == 6 && to_y == 4) // Double move (black)
            {
                new_board_state.board.black_pieces.pawns = move_bit(new_board_state.board.black_pieces.pawns, from_bitboard, to_bitboard);
                new_board_state.black_pieces = move_bit(new_board_state.black_pieces, from_bitboard, to_bitboard);
                new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
                new_board_state.board.en_passant = position_to_bitboard(to_x, to_y + 1);
            }
            else if (from_x != to_x && !is_white_piece(&new_board_state, to_x, to_y)) // En passant (black)
            {
                new_board_state.board.black_pieces.pawns = move_bit(new_board_state.board.black_pieces.pawns, from_bitboard, to_bitboard);
                new_board_state.black_pieces = move_bit(new_board_state.black_pieces, from_bitboard, to_bitboard);
                new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
                // Remove captured pawn (above the destination square)
                uint64_t capture_bitboard = new_board_state.board.en_passant << 8;
                new_board_state.board.white_pieces.pawns &= ~capture_bitboard;
                new_board_state.white_pieces &= ~capture_bitboard;
                new_board_state.occupied &= ~capture_bitboard;
                new_board_state.board.en_passant = 0;
            }
            else // Regular pawn move
            {
                new_board_state.board.black_pieces.pawns = move_bit(new_board_state.board.black_pieces.pawns, from_bitboard, to_bitboard);
                new_board_state.black_pieces = move_bit(new_board_state.black_pieces, from_bitboard, to_bitboard);
                new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
                new_board_state.board.en_passant = 0;
                if (is_capture_move(move))
                    remove_white_piece(&new_board_state, to_x, to_y);
            }
            break;
        case KNIGHT:
            new_board_state.board.black_pieces.knights = move_bit(new_board_state.board.black_pieces.knights, from_bitboard, to_bitboard);
            new_board_state.black_pieces = move_bit(new_board_state.black_pieces, from_bitboard, to_bitboard);
            new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
            new_board_state.board.en_passant = 0;
            if (is_capture_move(move))
                remove_white_piece(&new_board_state, to_x, to_y);
            break;
        case BISHOP:
            new_board_state.board.black_pieces.bishops = move_bit(new_board_state.board.black_pieces.bishops, from_bitboard, to_bitboard);
            new_board_state.black_pieces = move_bit(new_board_state.black_pieces, from_bitboard, to_bitboard);
            new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
            new_board_state.board.en_passant = 0;
            if (is_capture_move(move))
                remove_white_piece(&new_board_state, to_x, to_y);
            break;
        case ROOK:
            new_board_state.board.black_pieces.rooks = move_bit(new_board_state.board.black_pieces.rooks, from_bitboard, to_bitboard);
            new_board_state.black_pieces = move_bit(new_board_state.black_pieces, from_bitboard, to_bitboard);
            new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
            new_board_state.board.en_passant = 0;
            if (is_capture_move(move))
                remove_white_piece(&new_board_state, to_x, to_y);

            if (from_x == 0 && from_y == 7) // Queenside rook
                new_board_state.board.castling_rights &= ~BLACK_QUEENSIDE_CASTLE;
            if (from_x == 7 && from_y == 7) // Kingside rook
                new_board_state.board.castling_rights &= ~BLACK_KINGSIDE_CASTLE;

            break;
        case QUEEN:
            new_board_state.board.black_pieces.queens = move_bit(new_board_state.board.black_pieces.queens, from_bitboard, to_bitboard);
            new_board_state.black_pieces = move_bit(new_board_state.black_pieces, from_bitboard, to_bitboard);
            new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
            new_board_state.board.en_passant = 0;
            if (is_capture_move(move))
                remove_white_piece(&new_board_state, to_x, to_y);
            break;
        case KING:
            // Remove black castling rights when king moves
            new_board_state.board.castling_rights &= ~(BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE);

            if (from_x == 4 && from_y == 7 && to_x == 6 && to_y == 7) // Kingside castle (black)
            {
                // Move king
                new_board_state.board.black_pieces.king = move_bit(new_board_state.board.black_pieces.king, position_to_bitboard(4, 7), position_to_bitboard(6, 7));
                // Move rook
                new_board_state.board.black_pieces.rooks = move_bit(new_board_state.board.black_pieces.rooks, position_to_bitboard(7, 7), position_to_bitboard(5, 7));
                // Update aggregated bitboards
                new_board_state.black_pieces = (new_board_state.board.black_pieces.pawns |
                                                new_board_state.board.black_pieces.knights |
                                                new_board_state.board.black_pieces.bishops |
                                                new_board_state.board.black_pieces.rooks |
                                                new_board_state.board.black_pieces.queens |
                                                new_board_state.board.black_pieces.king);
                new_board_state.occupied = new_board_state.white_pieces | new_board_state.black_pieces;
                new_board_state.has_castled |= BLACK_KINGSIDE_CASTLE;
            }
            else if (from_x == 4 && from_y == 7 && to_x == 2 && to_y == 7) // Queenside castle (black)
            {
                // Move king
                new_board_state.board.black_pieces.king = move_bit(new_board_state.board.black_pieces.king, position_to_bitboard(4, 7), position_to_bitboard(2, 7));
                // Move rook
                new_board_state.board.black_pieces.rooks = move_bit(new_board_state.board.black_pieces.rooks, position_to_bitboard(0, 7), position_to_bitboard(3, 7));
                // Update aggregated bitboards
                new_board_state.black_pieces = (new_board_state.board.black_pieces.pawns |
                                                new_board_state.board.black_pieces.knights |
                                                new_board_state.board.black_pieces.bishops |
                                                new_board_state.board.black_pieces.rooks |
                                                new_board_state.board.black_pieces.queens |
                                                new_board_state.board.black_pieces.king);
                new_board_state.occupied = new_board_state.white_pieces | new_board_state.black_pieces;
                new_board_state.has_castled |= BLACK_QUEENSIDE_CASTLE;
            }
            else // Regular king move
            {
                new_board_state.board.black_pieces.king = move_bit(new_board_state.board.black_pieces.king, from_bitboard, to_bitboard);
                new_board_state.black_pieces = move_bit(new_board_state.black_pieces, from_bitboard, to_bitboard);
                new_board_state.occupied = move_bit(new_board_state.occupied, from_bitboard, to_bitboard);
                if (is_capture_move(move))
                    remove_white_piece(&new_board_state, to_x, to_y);
            }
            new_board_state.board.castling_rights &= ~(BLACK_KINGSIDE_CASTLE | BLACK_QUEENSIDE_CASTLE);
            new_board_state.board.en_passant = 0;
            break;
        }

        if (is_capture_move(move))
        {
            // check if rooks are not on start position. remove castling rights
            if (to_x == 0 && to_y == 0) // White queenside rook captured
                new_board_state.board.castling_rights &= ~WHITE_QUEENSIDE_CASTLE;
            if (to_x == 7 && to_y == 0) // White kingside rook captured
                new_board_state.board.castling_rights &= ~WHITE_KINGSIDE_CASTLE;
        }

        new_board_state.board.side_to_move = WHITE;
        // this is bad
        generate_white_attacks(board_state, &new_board_state);
        new_board_state.black_check = new_board_state.board.black_pieces.king & new_board_state.white_attack;
        if (new_board_state.black_check)
            return new_board_state;
        generate_black_attacks(board_state, &new_board_state);
        new_board_state.white_check = new_board_state.board.white_pieces.king & new_board_state.black_attack;
        ///
    }

    new_board_state.move = move;
    return new_board_state;
}

bool is_legal_move(const BoardState *board_state)
{
    if (board_state->board.side_to_move == WHITE)
        return board_state->black_check == false;
    else
        return board_state->white_check == false;
}