#include <stdint.h>

#include <drivers/uart.h>
#include <logbuffer.h>
#include <printk.h>

#include <arch/aarch64/gicv3.h>

struct log_buffer  printk_log_buffer;
struct log_buffer *printk_log_buffer_ptr;

void kmain(void)
{
    log_buffer_init(&printk_log_buffer);
    printk_log_buffer_ptr = &printk_log_buffer;

    uart_init();

    printk("-*- Vortex -*-\n");

    gicv3_init();
    gicv3_enable_interrupt(27); // Virtual Timer PPI

    // virtual_timer_enable();

    // init_virtio_mmio_device(&virtio, "idk yet",
    // (uint8_t *)0xa000000ULL); // from dts
}