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

static inline uint64_t position_to_u64(uint8_t x, uint8_t y)
{
    return 1ULL << (x + y * 8);
}

static inline uint64_t rank_mask(uint8_t y)
{
    return 0xFFULL << (y * 8);
}

static inline uint64_t file_mask(uint8_t x)
{
    return 0x0101010101010101ULL << x;
}

uint64_t flip_bitmap(uint64_t bitmap);
void print_bitboard(uint64_t bitboard);
bool has_timed_out(clock_t start, double seconds);