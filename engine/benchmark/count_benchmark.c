#include "count_benchmark.h"
#include "../../utils/fen.h"
#include <stdio.h>
#include <stdlib.h>
#include "../piece_moves.h"
#include "../tests/count_tests.h"

#if defined(_WIN32)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>

  static double now_seconds(void)
  {
      static LARGE_INTEGER freq;
      static int initialized = 0;
      if (!initialized) {
          QueryPerformanceFrequency(&freq);
          initialized = 1;
      }
      LARGE_INTEGER counter;
      QueryPerformanceCounter(&counter);
      return (double)counter.QuadPart / (double)freq.QuadPart;
  }

#else
  #include <time.h>

  static double now_seconds(void)
  {
  #if defined(CLOCK_MONOTONIC)
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
  #else
      // Fallback if clock_gettime isn't available
      return (double)clock() / (double)CLOCKS_PER_SEC;
  #endif
  }
#endif


uint64_t count_recursive(BoardState *board_state, uint8_t depth, BoardStack *stack)
{
    if (depth == 0)
        return 1;

    uint32_t base = stack->count;
    generate_moves(board_state, stack);

    if (stack->count == base)
        return 0;

    uint64_t total = 0;
    for (uint32_t i = base; i < stack->count; i++)
    {
        total += count_recursive(&stack->boards[i], depth - 1, stack);
    }

    stack->count = base;
    return total;
}

void run_count_benchmark()
{
    const size_t num_tests = sizeof(tests) / sizeof(Test);
    const size_t total_positions = num_tests * 2;

    // Pre-allocate BoardState array to avoid parsing inside the timed loop
    BoardState *states = malloc(total_positions * sizeof(BoardState));
    if (!states)
    {
        fprintf(stderr, "Failed to allocate states array\n");
        return;
    }

    // Fill in all positions (original + flipped)
    for (size_t i = 0; i < num_tests; i++)
    {
        Board b1 = fen_to_board(tests[i].fen);
        states[i] = board_to_board_state(&b1);
        Board b2 = flip_board(&b1);
        states[i + num_tests] = board_to_board_state(&b2);
    }

    BoardStack *stack = create_board_stack(BOARD_STACK_SIZE);
    if (!stack)
    {
        fprintf(stderr, "Failed to create board stack\n");
        free(states);
        return;
    }

    printf("%-7s %-13s %-11s %-17s %-17s\n",
           "Depth", "Nodes", "Time (s)", "Million boards/s", "Microseconds/board");

    for (uint8_t depth = 1; depth <= 3; depth++)
    {
        uint64_t total_nodes = 0;
        double t0 = now_seconds();

        for (size_t i = 0; i < total_positions; i++)
        {
            total_nodes += count_recursive(&states[i], depth, stack);
        }

        double t1 = now_seconds();
        double elapsed = t1 - t0;
        double mbps = total_nodes / (elapsed * 1e6);
        double us_per = (elapsed * 1e6) / (double)total_nodes;

        printf("%-7u %-13llu %-11.3f %-17.3f %-17.3f\n",
               depth, (unsigned long long)total_nodes,
               elapsed, mbps, us_per);
    }

    destroy_board_stack(stack);
    free(states);
}
