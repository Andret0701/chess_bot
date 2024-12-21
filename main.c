#include <stdio.h>
#include "board.h"
#include "engine/board_stack.h"
#include "engine/piece_moves.h"
#include "fen.h"

#include "engine/tests/count_test.h"
#include "engine/benchmark/count_benchmark.h"
#include "algorithm/min_max.h"
#include "algorithm/board_score.h"

int main()
{
    Board board = fen_to_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    BoardState board_state = board_to_board_state(&board);

    BoardStack *stack = create_board_stack(65535);

    clock_t start = clock();
    BoardScore alpha = get_worst_score(WHITE);
    BoardScore beta = get_worst_score(BLACK);
    SearchResult search_result = min_max(&board_state, stack, 11, 0, alpha, beta, start, 300.0);
    print_search_result(search_result);


    destroy_board_stack(stack);

    return 0;
}