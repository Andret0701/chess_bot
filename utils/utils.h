#pragma once

#include <stdint.h>
#include <time.h>
#include <stdbool.h>

static inline uint8_t popcountll(uint64_t x)
{
    return (uint8_t)__builtin_popcountll(x);
}

static inline uint8_t ctzll(uint64_t x)
{
    return (uint8_t)__builtin_ctzll(x);
}

bool has_timed_out(clock_t start, double seconds);