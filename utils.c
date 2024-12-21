#include "utils.h"
uint64_t flip_bitmap(uint64_t bitmap)
{ // only y should be flipped
    uint64_t result = 0;
    for (int i = 0; i < 64; i++)
    {
        int x = i % 8;
        int y = i / 8;
        y = 7 - y;
        if (bitmap & (1ULL << i))
            result |= 1ULL << (y * 8 + x);
    }
    return result;
}

void print_bitboard(uint64_t bitboard)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if (bitboard & (1ULL << square))
            {
                printf("[]");
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
    }

    printf("\n");
}

bool has_timed_out(clock_t start, double seconds)
{
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    if (cpu_time_used >= seconds - 0.001)
        return true;
    return false;
}