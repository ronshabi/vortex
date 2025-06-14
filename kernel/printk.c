#include <common/stringutils.h>
#include <drivers/uart.h>
#include <logbuffer.h>
#include <printk.h>
#include <stdarg.h>

#include <libc/ctype.h>
#include <libc/stdlib.h>
#include <libc/string.h>

#define PRINTK_CONV_BUFSIZE 1024

extern struct log_buffer *printk_log_buffer_ptr;

#define WRITE(str)  log_buffer_write(printk_log_buffer_ptr, (str))
#define WRITE_CH(c) log_buffer_writech(printk_log_buffer_ptr, (char)(c))
#define WRITE_CH_REP(c, n)                                                     \
    log_buffer_writech_repeating(printk_log_buffer_ptr, (char)(c), (size_t)(n))

enum printk_state
{
    NORMAL,
    PARSING_WIDTH,
    PARSING_FORMAT_SPECIFIER
};

static void uart_outfunc(int c)
{
    uart_write_ch(c);
}

static void pad_buffer(char *buf, size_t buf_len, int pad_width, char fill)
{
    const int  text_len = (int)strlen(buf);
    const bool is_left  = pad_width < 0;
    pad_width           = is_left ? -pad_width : pad_width;

    if (text_len >= pad_width)
    {
        return; // no padding needed
    }

    pad_width -= text_len;

    if ((size_t)text_len + (size_t)pad_width + 1 > buf_len)
    {
        // TODO: Signal error in a better way
        buf[0] = '^';
        buf[1] = '\0';
        return;
    }

    if (is_left)
    {
        memmove(buf + pad_width, buf, (size_t)text_len);
        memset8((uint8_t *)buf, (uint8_t)fill, (size_t)pad_width);
    }
    else
    {
        memset8((uint8_t *)buf + text_len, (uint8_t)fill, (size_t)pad_width);
    }

    buf[text_len + pad_width] = '\0';
}

__attribute__((format(printf, 1, 2))) void printk(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    enum printk_state state = NORMAL;

    char convbuf[PRINTK_CONV_BUFSIZE];

    char *string_arg           = NULL;
    char  curr                 = 0;
    char  curr1                = 0;
    int   fmt_extrainc         = 0;
    int   pad_width            = 0;
    bool  should_pad           = false;
    char  pad_filler           = ' ';
    bool  should_print_convbuf = true;

    while (*fmt)
    {
        fmt_extrainc = 0;
        curr         = *fmt++;
        curr1        = *fmt;

        switch (state)
        {
        case NORMAL:
            if (curr == '%')
            {
                if (isdigit(curr1))
                {
                    pad_width  = atoi(fmt);
                    pad_filler = (curr1 == '0') ? '0' : ' ';
                    should_pad = pad_width > 0;
                    state      = PARSING_WIDTH;
                }
                else
                {
                    should_pad = false;
                    pad_width  = 0;
                    state      = PARSING_FORMAT_SPECIFIER;
                }
            }
            else
            {
                WRITE_CH(curr);
            }

            break;

        case PARSING_WIDTH:
            if (!isdigit(curr1))
            {
                state = PARSING_FORMAT_SPECIFIER;
            }

            break;

        case PARSING_FORMAT_SPECIFIER:
            should_print_convbuf = true;
            if (curr == 'd')
            {
                string_from_int64(va_arg(ap, int32_t), convbuf,
                                  PRINTK_CONV_BUFSIZE);
            }
            else if (curr == 'l' && curr1 == 'd')
            {
                string_from_int64(va_arg(ap, int64_t), convbuf,
                                  PRINTK_CONV_BUFSIZE);
                fmt_extrainc = 1;
            }
            else if (curr == 'u')
            {
                string_from_uint64(va_arg(ap, uint32_t), 10, convbuf,
                                   PRINTK_CONV_BUFSIZE, false);
            }
            else if (curr == 'l' && curr1 == 'u')
            {
                string_from_uint64(va_arg(ap, uint64_t), 10, convbuf,
                                   PRINTK_CONV_BUFSIZE, false);
                fmt_extrainc = 1;
            }
            else if (curr == 'z')
            {
                string_from_uint64(va_arg(ap, size_t), 10, convbuf,
                                   PRINTK_CONV_BUFSIZE, false);
            }
            else if (curr == 'p')
            {
                string_from_uint64(va_arg(ap, uint64_t), 16, convbuf,
                                   PRINTK_CONV_BUFSIZE, false);
            }
            else if (curr == 'X')
            {
                string_from_uint64(va_arg(ap, uint32_t), 16, convbuf,
                                   PRINTK_CONV_BUFSIZE, false);
            }
            else if (curr == 'l' && curr1 == 'X')
            {
                string_from_uint64(va_arg(ap, uint64_t), 16, convbuf,
                                   PRINTK_CONV_BUFSIZE, false);
                fmt_extrainc = 1;
            }
            else if (curr == 'x')
            {
                string_from_uint64(va_arg(ap, uint32_t), 16, convbuf,
                                   PRINTK_CONV_BUFSIZE, true);
            }
            else if (curr == 'l' && curr1 == 'x')
            {
                string_from_uint64(va_arg(ap, uint64_t), 16, convbuf,
                                   PRINTK_CONV_BUFSIZE, true);
                fmt_extrainc = 1;
            }
            else if (curr == 'c')
            {
                should_print_convbuf = false;
                if (should_pad && pad_width < 0)
                {
                    WRITE_CH_REP(' ', (size_t)(-pad_width) - 1);
                }

                WRITE_CH(va_arg(ap, int));

                if (should_pad && pad_width - 1)
                {
                    log_buffer_writech_repeating(printk_log_buffer_ptr, ' ',
                                                 (size_t)pad_width - 1);
                }
            }
            else if (curr == 's')
            {
                should_print_convbuf     = false;
                string_arg               = va_arg(ap, char *);
                const int string_arg_len = (int)strlen(string_arg);

                if (should_pad && pad_width < 0 && string_arg_len < -pad_width)
                {
                    WRITE_CH_REP(' ',
                                 (size_t)(-pad_width) - (size_t)string_arg_len);
                }

                WRITE(string_arg);

                if (should_pad && pad_width && string_arg_len < pad_width)
                {
                    WRITE_CH_REP(' ',
                                 (size_t)pad_width - (size_t)string_arg_len);
                }
            }
            else
            {
                // error finding format specifier
                should_print_convbuf = false;
                WRITE_CH('?');
            }

            if (should_print_convbuf)
            {
                // zero padding implies left-padding
                if (pad_filler == '0')
                {
                    pad_width = -pad_width;
                }
                pad_buffer(convbuf, PRINTK_CONV_BUFSIZE, pad_width, pad_filler);
                WRITE(convbuf);
            }

            state = NORMAL;
            break;
        }

        fmt += fmt_extrainc;
    }

    va_end(ap);

    log_buffer_flush(printk_log_buffer_ptr, uart_outfunc);
}
