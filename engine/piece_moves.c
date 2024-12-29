#include "piece_moves.h"
#include "attack_generation/attack_generation.h"

{
    void init_board(BoardState * board_state)
        board_state->white_pieces = board_state->board.white_pieces.pawns | board_state->board.white_pieces.knights | board_state->board.white_pieces.bishops | board_state->board.white_pieces.rooks | board_state->board.white_pieces.queens | board_state->board.white_pieces.king;
    board_state->black_pieces = board_state->board.black_pieces.pawns | board_state->board.black_pieces.knights | board_state->board.black_pieces.bishops | board_state->board.black_pieces.rooks | board_state->board.black_pieces.queens | board_state->board.black_pieces.king;
    board_state->occupied = board_state->white_pieces | board_state->black_pieces;

    // check for castling rights
    if (board_state->board.castling_rights & WHITE_KINGSIDE_CASTLE && (board_state->board.white_pieces.rooks & position_to_u64(7, 0)) == 0)
        board_state->board.castling_rights &= ~WHITE_KINGSIDE_CASTLE;
    if (board_state->board.castling_rights & WHITE_QUEENSIDE_CASTLE && (board_state->board.white_pieces.rooks & position_to_u64(0, 0)) == 0)
        board_state->board.castling_rights &= ~WHITE_QUEENSIDE_CASTLE;
    if (board_state->board.castling_rights & BLACK_KINGSIDE_CASTLE && (board_state->board.black_pieces.rooks & position_to_u64(7, 7)) == 0)
        board_state->board.castling_rights &= ~BLACK_KINGSIDE_CASTLE;
    if (board_state->board.castling_rights & BLACK_QUEENSIDE_CASTLE && (board_state->board.black_pieces.rooks & position_to_u64(0, 7)) == 0)
        board_state->board.castling_rights &= ~BLACK_QUEENSIDE_CASTLE;

    generate_attacks(board_state);

    board_state->white_check = board_state->black_attack & board_state->board.white_pieces.king;
    board_state->black_check = board_state->white_attack & board_state->board.black_pieces.king;
}

void validate_white_move(BoardStack *stack)
{
    if (!stack->boards[stack->count].white_check)
        stack->count++;
}

void validate_black_move(BoardStack *stack)
{
    if (!stack->boards[stack->count].black_check)
        stack->count++;
}

void remove_white_piece(BoardState *board_state, uint8_t x, uint8_t y)
{
    uint64_t position = position_to_u64(x, y);
    board_state->board.white_pieces.pawns &= ~position;
    board_state->board.white_pieces.knights &= ~position;
    board_state->board.white_pieces.bishops &= ~position;
    board_state->board.white_pieces.rooks &= ~position;
    board_state->board.white_pieces.queens &= ~position;
}
void remove_black_piece(BoardState *board_state, uint8_t x, uint8_t y)
{
    uint64_t position = position_to_u64(x, y);
    board_state->board.black_pieces.pawns &= ~position;
    board_state->board.black_pieces.knights &= ~position;
    board_state->board.black_pieces.bishops &= ~position;
    board_state->board.black_pieces.rooks &= ~position;
    board_state->board.black_pieces.queens &= ~position;
}

void generate_moves(BoardState *board_state, BoardStack *stack)
{
    if (board_state->board.side_to_move == WHITE)
    {
        for (uint8_t y = 0; y < 8; y++)
        {
            for (uint8_t x = 0; x < 8; x++)
            {
                uint64_t position = position_to_u64(x, y);
                if (board_state->white_pieces & position)
                {
                    if (board_state->board.white_pieces.pawns & position)
                        generate_white_pawn_moves(board_state, x, y, stack);
                    else if (board_state->board.white_pieces.knights & position)
                        generate_white_knight_moves(board_state, x, y, stack);
                    else if (board_state->board.white_pieces.bishops & position)
                        generate_white_bishop_moves(board_state, x, y, stack);
                    else if (board_state->board.white_pieces.rooks & position)
                        generate_white_rook_moves(board_state, x, y, stack);
                    else if (board_state->board.white_pieces.queens & position)
                        generate_white_queen_moves(board_state, x, y, stack);
                    else
                        generate_white_king_moves(board_state, x, y, stack);
                }
            }
        }
    }
    else if (board_state->board.side_to_move == BLACK)
    {
        for (uint8_t y = 0; y < 8; y++)
        {
            for (uint8_t x = 0; x < 8; x++)
            {
                uint64_t position = position_to_u64(x, y);
                if (board_state->black_pieces & position)
                {
                    if (board_state->board.black_pieces.pawns & position)
                        generate_black_pawn_moves(board_state, x, y, stack);
                    else if (board_state->board.black_pieces.knights & position)
                        generate_black_knight_moves(board_state, x, y, stack);
                    else if (board_state->board.black_pieces.bishops & position)
                        generate_black_bishop_moves(board_state, x, y, stack);
                    else if (board_state->board.black_pieces.rooks & position)
                        generate_black_rook_moves(board_state, x, y, stack);
                    else if (board_state->board.black_pieces.queens & position)
                        generate_black_queen_moves(board_state, x, y, stack);
                    else
                        generate_black_king_moves(board_state, x, y, stack);
                }
            }
        }
    }
}

void generate_captures(BoardState *board_state, BoardStack *stack)
{
    if (board_state->board.side_to_move == WHITE)
    {
        for (uint8_t y = 0; y < 8; y++)
        {
            for (uint8_t x = 0; x < 8; x++)
            {
                uint64_t position = position_to_u64(x, y);
                if (board_state->white_pieces & position)
                {
                    if (board_state->board.white_pieces.pawns & position)
                        generate_white_pawn_captures(board_state, x, y, stack);
                    else if (board_state->board.white_pieces.knights & position)
                        generate_white_knight_captures(board_state, x, y, stack);
                    else if (board_state->board.white_pieces.bishops & position)
                        generate_white_bishop_captures(board_state, x, y, stack);
                    else if (board_state->board.white_pieces.rooks & position)
                        generate_white_rook_captures(board_state, x, y, stack);
                    else if (board_state->board.white_pieces.queens & position)
                        generate_white_queen_captures(board_state, x, y, stack);
                    else
                        generate_white_king_captures(board_state, x, y, stack);
                }
            }
        }
    }
    else if (board_state->board.side_to_move == BLACK)
    {
        for (uint8_t y = 0; y < 8; y++)
        {
            for (uint8_t x = 0; x < 8; x++)
            {
                uint64_t position = position_to_u64(x, y);
                if (board_state->black_pieces & position)
                {
                    if (board_state->board.black_pieces.pawns & position)
                        generate_black_pawn_captures(board_state, x, y, stack);
                    else if (board_state->board.black_pieces.knights & position)
                        generate_black_knight_captures(board_state, x, y, stack);
                    else if (board_state->board.black_pieces.bishops & position)
                        generate_black_bishop_captures(board_state, x, y, stack);
                    else if (board_state->board.black_pieces.rooks & position)
                        generate_black_rook_captures(board_state, x, y, stack);
                    else if (board_state->board.black_pieces.queens & position)
                        generate_black_queen_captures(board_state, x, y, stack);
                    else
                        generate_black_king_captures(board_state, x, y, stack);
                }
            }
        }
    }
}
