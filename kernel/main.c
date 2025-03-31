#include <stdint.h>

#include "printk.h"
#include "uart.h"

#include "logbuffer.h"

struct log_buffer  printk_log_buffer;
struct log_buffer *printk_log_buffer_ptr;

void
kmain(void)
{
    log_buffer_init(&printk_log_buffer);
    printk_log_buffer_ptr = &printk_log_buffer;

    printk("hello world %x\n", 0xdeadbeef);

    // do some funny shit

    volatile int *x = (int *)0x7F7F7F7F;
    *x              = 20;
}