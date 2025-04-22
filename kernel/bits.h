#pragma once

#define BIT(n)                (1 << (n))
#define BIT_ALL_EXCEPT(n)     (~(1U << (n)))
#define BIT_GET(value, bitno) (((value) >> (bitno)) & 1)