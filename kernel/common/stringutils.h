#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void string_from_uint64(uint64_t value, uint32_t base, char *outbuf,
                        size_t outbuf_size, bool hexlower);

void string_from_int64(int64_t value, char *outbuf, size_t outbuf_size);

void string_reverse(char *buf, size_t buf_size);