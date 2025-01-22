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