#include "uart.h"
#include "memory.h"

volatile uint8_t *aarch64_virt_uart_addr = (uint8_t *)0x09000000;

#define ARM_PL011_BASE 0x09000000
#define ARM_PL011_UARTDR ARM_PL011_BASE
#define ARM_PL011_UARTCR (ARM_PL011_BASE + 0x030UL) // ARM MANUAL



void
uart_init(void)
{
    // https://developer.arm.com/documentation/ddi0183/g/programmers-model/register-descriptions/control-register--uartcr
    mem_write_u32(ARM_PL011_UARTCR, 0b0000001100000001);
}

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
