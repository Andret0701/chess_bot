#include "piece_moves.h"

void init_board(BoardState *board_state)
{
    board_state->white_pieces = board_state->board.white_pieces.pawns | board_state->board.white_pieces.knights | board_state->board.white_pieces.bishops | board_state->board.white_pieces.rooks | board_state->board.white_pieces.queens | board_state->board.white_pieces.king;
    board_state->black_pieces = board_state->board.black_pieces.pawns | board_state->board.black_pieces.knights | board_state->board.black_pieces.bishops | board_state->board.black_pieces.rooks | board_state->board.black_pieces.queens | board_state->board.black_pieces.king;
}
void validate_white_move(BoardStack *stack)
{
    stack->count++;
}

void validate_black_move(BoardStack *stack)
{
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

void generate_moves(Board *board, BoardStack *stack)
{
    BoardState *board_state = &stack->boards[stack->count];
    printf("generate_moves\n");
    copy_board(board, &board_state->board);
    printf("copy_board\n");
    stack->count++;

    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            printf("x: %d, y: %d\n", x, y);
            uint64_t position = position_to_u64(x, y);
            if (board->white_pieces.pawns & position)
                generate_white_pawn_moves(board_state, x, y, stack);
            else if (board->black_pieces.pawns & position)
                generate_black_pawn_moves(board_state, x, y, stack);
            else if (board->white_pieces.knights & position)
                generate_white_knight_moves(board_state, x, y, stack);
            else if (board->black_pieces.knights & position)
                generate_black_knight_moves(board_state, x, y, stack);
            else if (board->white_pieces.bishops & position)
                generate_white_bishop_moves(board_state, x, y, stack);
            else if (board->black_pieces.bishops & position)
                generate_black_bishop_moves(board_state, x, y, stack);
            else if (board->white_pieces.rooks & position)
                generate_white_rook_moves(board_state, x, y, stack);
            else if (board->black_pieces.rooks & position)
                generate_black_rook_moves(board_state, x, y, stack);
            else if (board->white_pieces.queens & position)
                generate_white_queen_moves(board_state, x, y, stack);
            else if (board->black_pieces.queens & position)
                generate_black_queen_moves(board_state, x, y, stack);
            else if (board->white_pieces.king & position)
                generate_white_king_moves(board_state, x, y, stack);
            else if (board->black_pieces.king & position)
                generate_black_king_moves(board_state, x, y, stack);
        }
    }
}