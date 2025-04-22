#include "stringutils.h"
#include "klibc.h"

void
string_from_uint64(uint64_t value, uint32_t base, char *outbuf,
                   size_t outbuf_size, bool hexlower)
{
    size_t i = 0;
    memset8((uint8_t *)outbuf, 0, outbuf_size);
    const char *c_digits = "0123456789ABCDEFabcdef";

    if (value == 0)
    {
        outbuf[0] = '0';
        return;
    }

    while (value)
    {
        outbuf[i] = c_digits[(value % (uint64_t)base)];

        if (outbuf[i] >= 'A' && outbuf[i] <= 'F' && hexlower) {
            outbuf[i] += 'a' - 'A';
        }

        value /= base;
        ++i;
    }

    string_reverse(outbuf, outbuf_size);
}

void
string_from_int64(int64_t value, char *outbuf, size_t outbuf_size)
{
    size_t i                = 0;
    bool   should_add_minus = false;
    memset8((uint8_t *)outbuf, 0, outbuf_size);
    const char *c_digits = "0123456789";

    if (value == 0)
    {
        outbuf[0] = '0';
        return;
    }

    if (value < 0)
    {
        should_add_minus = true;
    }

    while (value)
    {
        outbuf[i] = c_digits[value % 10];
        value /= 10;
        ++i;
    }

    if (should_add_minus && i + 1 < outbuf_size)
    {
        outbuf[i] = '-';
    }

    string_reverse(outbuf, outbuf_size);
}

void
string_reverse(char *buf, size_t buf_size)
{
    size_t len = strnlen(buf, buf_size);

    for (size_t i = 0; i < len / 2; ++i)
    {
        char temp        = buf[i];
        buf[i]           = buf[len - i - 1];
        buf[len - i - 1] = temp;
    }
}
