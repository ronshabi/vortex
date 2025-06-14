#include <stdint.h>

static inline uint32_t bswap_32(uint32_t x)
{
    uint32_t o = 0;
    o |= ((x & 0xFF) << 24);
    o |= (((x >> 8) & 0xFF) << 16);
    o |= (((x >> 16) & 0xFF) << 8);
    o |= ((x >> 24) & 0xFF);
    return o;
}