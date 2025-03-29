#include "printk.h"
#include "uart.h"

__attribute__((format(printf, 1, 2))) void printk(const char *fmt, ...)
{
    uart_write(fmt);
}
