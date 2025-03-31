#include "printk.h"
#include "uart.h"

#include "logbuffer.h"
extern struct log_buffer *printk_log_buffer_ptr;

static void uart_outfunc(int c)
{
    uart_write_ch(c);
}

__attribute__((format(printf, 1, 2))) void printk(const char *fmt, ...)
{
    log_buffer_writestr(printk_log_buffer_ptr, fmt);
    log_buffer_flush(printk_log_buffer_ptr, uart_outfunc);
}

