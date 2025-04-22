#include <stdint.h>

#include "gic_v3.h"
#include "uart.h"
#include "printk.h"

#include "logbuffer.h"

struct log_buffer  printk_log_buffer;
struct log_buffer *printk_log_buffer_ptr;

void
kmain(void)
{
    log_buffer_init(&printk_log_buffer);
    printk_log_buffer_ptr = &printk_log_buffer;

    uart_init();

    printk("-*- Vortex -*-\n");
    

    init_gic();
    enable_timer();
}