#include "utils.h"

// Count the number of 1 bits in an unsigned long long integer
uint8_t popcountll(uint64_t x)
{
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    x = x + (x >> 8);
    x = x + (x >> 16);
    x = x + (x >> 32);
    return x & 0x7F;
}

// Count trailing zeros in an unsigned long long integer
uint8_t ctzll(uint64_t x)
{
    if (x == 0)
        return 64;

    uint8_t count = 0;
    while ((x & 1) == 0)
    {
        x >>= 1;
        count++;
    }

    return count;
}

uint64_t position_to_u64(uint8_t x, uint8_t y)
{
    return 1ULL << (y * 8ULL + x);
}

uint64_t rank_mask(uint8_t y)
{
    return 0xFFULL << (y * 8);
}

uint64_t file_mask(uint8_t x)
{
    return 0x0101010101010101ULL << x;
}

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