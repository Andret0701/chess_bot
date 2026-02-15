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
uint8_t position_to_index(uint8_t x, uint8_t y);
uint32_t clamp(int32_t value, int32_t min, int32_t max);