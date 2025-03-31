#pragma once

#include <stddef.h>
#include <stdint.h>

void
memset8(uint8_t *ptr, uint8_t c, size_t size);
void *
memcpy(void *dest, const void *src, size_t n);
void *
memmove(void *dest, const void *src, size_t n);
size_t
strlen(const char *s);
size_t
strnlen(const char *s, size_t n);
char *
strcpy(char *dest, const char *src);

static inline int
isdigit(int c)
{
    return c >= '0' && c <= '9';
}

int
atoi(const char *nptr);