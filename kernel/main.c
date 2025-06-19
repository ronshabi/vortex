#include "arch/aarch64/timer.h"
#include <devicetree/devicetree.h>
#include "drivers/pci.h"
#include <stdint.h>

#include <drivers/uart.h>
#include <logbuffer.h>
#include <printk.h>

#include <arch/aarch64/gicv3.h>

struct log_buffer  printk_log_buffer;
struct log_buffer *printk_log_buffer_ptr;

// struct devicetree devicetree;

#define KMEM_BASE 0x40200000
#define KMEM_SIZE 0x100000      // 1MiB

void do_exception(void) {
    void (*p)(void) = (void*)0;
    p();
}

void kmain(void)
{
    log_buffer_init(&printk_log_buffer);
    printk_log_buffer_ptr = &printk_log_buffer;

    uart_init();

    printk("-*- Vortex -*-\n");

    // devicetree_init(&devicetree, 0x40000000);
    // devicetree_parse(&devicetree);

    do_exception();

    // virtual_timer_enable();


    // gicv3_init();
    // gicv3_enable_interrupt(27); // Virtual Timer PPI


    // init_virtio_mmio_device(&virtio, "idk yet",
    // (uint8_t *)0xa000000ULL); // from dts

    // pci_init();
}