#include <stdint.h>

#include "logbuffer.h"
#include "pci.h"
#include "printk.h"
#include "uart.h"
#include "virtio.h"

struct log_buffer  printk_log_buffer;
struct log_buffer *printk_log_buffer_ptr;

struct virtio_mmio_device virtio;

void kmain(void)
{
    log_buffer_init(&printk_log_buffer);
    printk_log_buffer_ptr = &printk_log_buffer;

    uart_init();

    printk("-*- Vortex -*-\n");

    // init_gic();
    // virtual_timer_enable();

    // init_virtio_mmio_device(&virtio, "idk yet",
    // (uint8_t *)0xa000000ULL); // from dts

    pci_init();
}