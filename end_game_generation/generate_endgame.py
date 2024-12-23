import chess
import itertools

import chess.pgn
import chess.engine

from stockfish import Stockfish

# Initialize Stockfish with the path to the Stockfish executable
stockfish = Stockfish("C:/Users/andre/OneDrive/Dokumenter/stockfish/stockfish-windows-x86-64-avx2.exe")

#make sure the engine is ready
stockfish.set_depth(25)

def generate_king_positions():
    positions = []
    for i in range(64):
        for j in range(64):
            if i == j:
                continue
            for turn in ['w', 'b']:
                board = chess.Board(fen=None)
                board.clear_board()
                board.turn = chess.WHITE if turn == 'w' else chess.BLACK
                # Place the kings first
                board.set_piece_at(i, chess.Piece.from_symbol('K'))
                board.set_piece_at(j, chess.Piece.from_symbol('k'))
                if board.is_valid():
                    positions.append(board.fen())

    return positions


def add_piece(positions: list):
    new_positions = []
    for piece in ['p', 'r', 'n', 'b', 'q','P', 'R', 'N', 'B', 'Q']:
        for i in range(64):
            for position in positions:
                board = chess.Board(fen=position)
                if board.piece_at(i) is not None:
                    continue
                board.set_piece_at(i, chess.Piece.from_symbol(piece))
                if board.is_valid():
                    new_positions.append(board.fen())
    return new_positions

def get_winner(fen):
    board = chess.Board(fen=fen)
    if board.is_checkmate():
        result = board.result()
        if result == "1-0":
            return "white", 0
        elif result == "0-1":
            return "black", 0
        elif result == "1/2-1/2":
            return "draw", 0
    
    stockfish.set_fen_position(fen)
    evaluation = stockfish.get_evaluation()
    if evaluation["type"] == "mate":
        if evaluation["value"] > 0:
            return "white", evaluation["value"]
        elif evaluation["value"] < 0:
            return "black", evaluation["value"]
    return "draw", evaluation["value"]


def has_insufficient_material(board: chess.Board) -> bool:
    """
    Check if the position on the board has insufficient material to continue the game.

    Args:
        board (chess.Board): The chess board to evaluate.

    Returns:
        bool: True if the position has insufficient material, False otherwise.
    """
    # Check for pawns, rooks, or queens - if any exist, material is sufficient
    for piece_type in [chess.PAWN, chess.ROOK, chess.QUEEN]:
        if board.pieces(piece_type, chess.WHITE) or board.pieces(piece_type, chess.BLACK):
            return False

    # Count minor pieces (knights and bishops)
    white_knights = len(board.pieces(chess.KNIGHT, chess.WHITE))
    black_knights = len(board.pieces(chess.KNIGHT, chess.BLACK))
    white_bishops = len(board.pieces(chess.BISHOP, chess.WHITE))
    black_bishops = len(board.pieces(chess.BISHOP, chess.BLACK))

    # King vs King
    if white_knights + black_knights + white_bishops + black_bishops == 0:
        return True

    # King + minor piece vs King
    if white_knights + black_knights + white_bishops + black_bishops == 1:
        return True

    # King + Bishop vs King + Bishop (check if bishops are on same color squares)
    if white_knights + black_knights == 0 and white_bishops == 1 and black_bishops == 1:
        white_bishop_square = next(iter(board.pieces(chess.BISHOP, chess.WHITE)))
        black_bishop_square = next(iter(board.pieces(chess.BISHOP, chess.BLACK)))

        # Determine square color (light or dark)
        white_bishop_on_light = (white_bishop_square % 2) == ((white_bishop_square // 8) % 2)
        black_bishop_on_light = (black_bishop_square % 2) == ((black_bishop_square // 8) % 2)

        return white_bishop_on_light == black_bishop_on_light

    return False


def filter_positions(positions: list) -> list:
    return [position for position in positions if not has_insufficient_material(chess.Board(fen=position))]

if __name__ == "__main__":
    positions = generate_king_positions()
    three_pieces = add_piece(positions)
    #four_pieces = add_piece(three_pieces)
    positions = positions + three_pieces
    print(len(positions))
    positions = filter_positions(positions)
    print(len(positions))
    # Evaluate the positions
    with open("evaluated_positions.txt", "w") as file:
        for position in positions:
            winner = get_winner(position)
            file.write(f"{position}, {winner}\n")
            print(f"{position}, {winner}")
    
    
    