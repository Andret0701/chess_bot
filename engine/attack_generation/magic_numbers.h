#pragma once

#include <stdint.h>

typedef struct
{
    uint64_t magic;
    uint8_t shift;
    uint64_t mask;
    uint32_t offset;
} MagicNumber;