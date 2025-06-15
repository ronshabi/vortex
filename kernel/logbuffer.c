#include "logbuffer.h"
#include <klibc/string.h>

void log_buffer_init(struct log_buffer *lb)
{
    memset8((uint8_t *)lb->buffer, 0, LOG_BUFFER_SIZE);
    lb->head = 0;
    lb->tail = 0;
}

int log_buffer_writech(struct log_buffer *lb, char c)
{
    const size_t next = (lb->head + 1) % LOG_BUFFER_SIZE;

    if (next != lb->tail)
    {
        lb->buffer[lb->head] = c;
        lb->head             = (lb->head + 1) % LOG_BUFFER_SIZE;
        return 0;
    }

    return 1;
}

int log_buffer_writech_repeating(struct log_buffer *lb, char c, size_t n)
{
    int res = 0;

    while (n--)
    {
        res |= log_buffer_writech(lb, c);
    }

    return res;
}

int log_buffer_write(struct log_buffer *lb, const char *str)
{
    int res = 0;
    while (*str)
    {
        res |= log_buffer_writech(lb, *str);
        ++str;
    }

    return res;
}

int log_buffer_readch(struct log_buffer *lb)
{
    if (lb->tail == lb->head)
    {
        return 0;
    }

    const int o = lb->buffer[lb->tail];
    lb->tail    = (lb->tail + 1) % LOG_BUFFER_SIZE;
    return o;
}

void log_buffer_flush(struct log_buffer *lb, void (*outfunc)(int c))
{
    while (lb->tail != lb->head)
    {
        const int c = log_buffer_readch(lb);
        if (!c)
        {
            break;
        }

        outfunc(c);
    }
}
