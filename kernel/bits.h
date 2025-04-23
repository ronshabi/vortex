#pragma once

#include <stdint.h>

#define BIT(n)                (1 << (n))
#define BIT_ALL_EXCEPT(n)     (~(1U << (n)))
#define BIT_GET(value, bitno) (((value) >> (bitno)) & 1)
#define BIT_MASK(n)           ((1UL << (n)) - 1UL)

// Extract bits (LSB=0) [from:to) -- excluding `to`
static inline uint64_t bits_extract_u64(uint64_t value, uint32_t from,
                                        uint32_t to)
{
    return (value >> from) & BIT_MASK(to - from);
}