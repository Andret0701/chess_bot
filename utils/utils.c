#include "utils.h"
#include <stdio.h>

bool has_timed_out(clock_t start, double seconds)
{
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    if (cpu_time_used >= seconds - 0.001)
        return true;
    return false;
}

uint8_t position_to_index(uint8_t x, uint8_t y)
{
    return (x + y * 8);
}