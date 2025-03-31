#include "klibc.h"

void memset8(uint8_t *ptr, uint8_t c, uint64_t size)
{
    for (uint64_t i = 0; i < size; ++i) {
        ptr[i] = c;
    }
}
