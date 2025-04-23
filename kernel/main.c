#include <stdint.h>

#include "gicv3.h"
#include "logbuffer.h"
#include "printk.h"
#include "timer.h"
#include "uart.h"

struct log_buffer  printk_log_buffer;
struct log_buffer *printk_log_buffer_ptr;

void kmain(void)
{
    log_buffer_init(&printk_log_buffer);
    printk_log_buffer_ptr = &printk_log_buffer;

    uart_init();

    printk("-*- Vortex -*-\n");
    init_gic();
    virtual_timer_enable();
}