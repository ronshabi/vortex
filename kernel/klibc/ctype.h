#ifndef VORTEX_KLIBC_CTYPE__
#define VORTEX_KLIBC_CTYPE__
#include <stddef.h>
#include <stdint.h>

static inline int isdigit(int c)
{
    return c >= '0' && c <= '9';
}

#endif