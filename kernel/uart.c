#include "uart.h"

volatile uint8_t *aarch64_virt_uart_addr = (uint8_t *)0x09000000;

void
uart_write(const char *buf)
{
    while (*buf)
    {
        uart_write_ch(*buf);
        ++buf;
    }
}

void
uart_write_ch(int ch)
{
    *aarch64_virt_uart_addr = (uint8_t)ch;
}
