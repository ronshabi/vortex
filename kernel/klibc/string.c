#include "string.h"

void memset8(uint8_t *ptr, uint8_t c, uint64_t size)
{
    for (uint64_t i = 0; i < size; ++i)
    {
        ptr[i] = c;
    }
}

void *memcpy(void *dest, const void *src, size_t n)
{
    uint8_t       *d = dest;
    const uint8_t *s = src;

    if (d == s)
    {
        return dest;
    }

    while (n--)
    {
        *d = *s;
        d++;
        s++;
    }

    return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
    // aliasing
    uint8_t       *d = dest;
    const uint8_t *s = src;

    if (d == s)
    {
        return dest;
    }

    // no overlap
    if ((uintptr_t)s - (uintptr_t)d <= (uintptr_t)(-n))
    {
        return memcpy(d, s, n);
    }

    if (d < s)
    {
        // forward-copy
        while (n--)
        {
            *d = *s;
            s++;
            d++;
        }
    }
    else
    {
        // backward-copy
        while (n--)
        {
            d[n] = s[n];
        }
    }

    return dest;
}

size_t strlen(const char *s)
{
    return strnlen(s, SIZE_MAX);
}

size_t strnlen(const char *s, size_t n)
{
    size_t count = 0;

    while (*s && count < n)
    {
        ++count;
        ++s;
    }

    return count;
}

char *strcpy(char *dest, const char *src)
{
    while (*src)
    {
        *dest++ = *src++;
    }

    *dest = '\0';

    return (char *)src;
}