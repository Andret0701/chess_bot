import chess
import itertools

import chess.pgn
import chess.engine

from stockfish import Stockfish
from tqdm import tqdm



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
                    positions.append(board)

    return positions


def add_piece(positions: list):
    new_positions = []
    pieces = ['p', 'r', 'n', 'b', 'q', 'P', 'R', 'N', 'B', 'Q']
    total = len(pieces) * 64 * len(positions)
    with tqdm(total=total, desc="Adding pieces") as pbar:
        for piece in pieces:
            for i in range(64):
                for position in positions:
                    board = position.copy()
                    if board.piece_at(i) is not None:
                        pbar.update(1)
                        continue
                    board.set_piece_at(i, chess.Piece.from_symbol(piece))
                    if board.is_valid() and not board.is_game_over():
                        new_positions.append(board)
                    pbar.update(1)
    return new_positions

def get_winner(board: chess.Board):
    if board.is_game_over():
        result = board.result()
        if result == "1-0":
            return "white"
        elif result == "0-1":
            return "black"
        elif result == "1/2-1/2":
            return "draw"
    
    return "unknown"


def choose_result(color: str, result1: tuple[str, int], result2: tuple[str, int]) -> tuple[str, int]:
    """
    Pick the more desirable (outcome, depth) for the side `color`.

    Outcome meanings
    ----------------
    * color         – a win for the side we care about
    * draw          – stalemate / repetition / etc.
    * other colour  – a loss for `color`
    * unknown       – the search hasn’t resolved this branch

    Depth is the number of plies to reach the outcome (0 = immediate).

    Policy
    ------
    1. Prefer a win over anything else.
       • For wins, an earlier mate is better (smaller depth).
    2. Prefer a draw over a loss or unknown.
       • Earlier draws are better (smaller depth).
    3. Prefer “unknown” over a confirmed loss (there is still hope).
       • Deeper unknowns are *slightly* better than shallow ones.
    4. Between two losses, delay defeat as long as possible
       (larger depth is better).
    """
    def score(res: tuple[str, int]) -> tuple[int, int]:
        outcome, depth = res
        if outcome == color:                # guaranteed win
            return (3, -depth)              # bigger first element beats smaller;
                                            # negate depth so earlier-win > later-win
        if outcome == "draw":               # draw
            return (2, -depth)
        if outcome == "unknown":            # maybe good, maybe bad
            return (1, -depth)              # prefer deeper “unknown” slightly
        # everything else is a loss for `color`
        return (0, depth)                   # delay the loss ⇒ larger depth preferred

    # max() with the custom key picks the higher-scoring result
    return max(result1, result2, key=score)

    
    

if __name__ == "__main__":
    boards = generate_king_positions()
    print(len(boards))
    boards = add_piece(boards)
    print(len(boards))

    results = {}
    for board in boards:
        results[board.fen()] = ("unknown", 0)
    print("Starting evaluation...")
    
    for board in tqdm(boards, desc="Evaluating boards"):
        for move in board.legal_moves:
            new_board = board.copy()
            new_board.push(move)
            result = get_winner(new_board)
            if result == "unknown":
                result = results[new_board.fen()]
            side = "white" if board.turn == chess.WHITE else "black" 
            results[board.fen()] = choose_result(side,results[board.fen()],result)

            
    print("Evaluation complete.")
    
    count_not_unknown = 0
    for board in tqdm(boards, desc="Counting unknown boards"):
        if results[board.fen()][0]!="unknown":
            count_not_unknown+=1
            
    print(count_not_unknown)
    