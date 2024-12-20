#pragma once

#include <stdint.h>

uint8_t popcountll(uint64_t x);
uint8_t ctzll(uint64_t x);
uint64_t position_to_u64(uint8_t x, uint8_t y);
uint64_t rank_mask(uint8_t y);
uint64_t file_mask(uint8_t x);
uint64_t flip_bitmap(uint64_t bitmap);
void print_bitboard(uint64_t bitboard);