#include <stdint.h>

#include "gicv3.h"
#include "uart.h"
#include "printk.h"
#include "memory.h"
#include "bits.h"

#include "logbuffer.h"

struct log_buffer  printk_log_buffer;
struct log_buffer *printk_log_buffer_ptr;

void
print_system_counter()
{
    uint32_t tm = 0;
    __asm__ volatile ("mrs %0, CNTV_TVAL_EL0" : "=r"(tm));
    printk("syscounter: %X\n", tm);
}

void get_cntv_ctl_el0()
{
    uint64_t val;
    __asm__ volatile ("mrs %0, CNTV_CTL_EL0" : "=r"(val));
    printk("timer ctl: %lX\n", val);
}

void get_daif()
{
    uint64_t daif = 0;
    __asm__ volatile("mrs %0, DAIF" : "=r"(daif));
    printk("DAIF = %lX\n", daif >> 6);
}


void aarch64_GICR_TYPER_print_info() {
    const uint64_t reg = mem_read_u64(GICR_REG_TYPER);

    printk(" - GICR_TYPER: 0x%lX\n", reg);

    const uint32_t affinity = (uint32_t)bits_extract_u64(reg, 32, 64);

    printk(" - Affinity Value: 0x%X\n", affinity);

    printk(" - PLPIS[0] = %lu\n", bits_extract_u64(reg, 0, 1));
    printk(" - DirectLPI[3] = %lu\n", bits_extract_u64(reg, 3, 4));
    printk(" - Last[4] = %lu\n", bits_extract_u64(reg, 4, 5));
    printk(" - DS[6] = %lu\n", bits_extract_u64(reg, 6, 7));
    printk(" - PPInum[31:27] = %lu\n", bits_extract_u64(reg, 27, 32));
}

void
kmain(void)
{
    log_buffer_init(&printk_log_buffer);
    printk_log_buffer_ptr = &printk_log_buffer;
    
    uart_init();

    printk("-*- Vortex -*-\n");

    // aarch64_GICR_TYPEaR_print_info();


    // get_cntv_ctl_el0();

    init_gic();

    // aarch64_GICR_TYPER_print_info();

    // enable_timer();
    // print_system_counter();

    // aarch64_GICR_TYPER_print_info();

    // get_cntv_ctl_el0();

}