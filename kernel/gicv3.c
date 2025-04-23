#include "gicv3.h"
#include "aarch64.h"
#include "memory.h"
#include "printk.h"
#include "bits.h"

// GIC Redistributor: Mark PEs as online
static void gicr_wakeup()
{
    volatile uint32_t current_waker = mem_read_u32(GICR_REG_WAKER);
    
    // Mark `ProcessorSleep` (bit 1) to be 0
    mem_write_u32(GICR_REG_WAKER, current_waker & BIT_ALL_EXCEPT(1));

    // Poll `ChildrenAsleep` (bit 2) until it is zero
    while (1) {
        current_waker = mem_read_u32(GICR_REG_WAKER);
        if (BIT_GET(current_waker, 2) == 0) {
            printk("GICv3: PE is online\n");
            break;
        }
    }
}


void init_gic()
{
    printk("GICv3: Start initialization\n");
    gicr_wakeup();

    // set virtual timer ppi (IRQ 27) to be in group1 (ns) and unmask it
    mem_write_u32(GICR_REG_SGI_ISENABLER0, BIT(27));
    mem_write_u32(GICR_REG_SGI_IGROUPR0, mem_read_u32(GICR_REG_SGI_IGROUPR0) | BIT(27));

    // enable sysreg for controlling the interrupt controller 
    uint64_t ICC_SRE_EL1 = 0;
    __asm__ volatile ("mrs %0, ICC_SRE_EL1" : "=r"(ICC_SRE_EL1));
    __asm__ volatile ("msr ICC_SRE_EL1, %0" : : "r"(ICC_SRE_EL1 | BIT(0)));
    __asm__ volatile ("isb");

    // unmask priority using sysreg
    const uint64_t FF = 0xff; // allow all to pass thru, because 0 is the highest prio
    __asm__ volatile ("msr ICC_PMR_EL1, %0" : : "r"(FF));

    // enable group1 (ns) delivery
    const uint32_t ONE = 1;
    __asm__ volatile ("msr ICC_IGRPEN1_EL1, %0" : : "r"(ONE));
    __asm__ volatile ("isb");
    
    // enable the distributor
    mem_write_u32(GICD_REG_CTLR, 0x2);
    __asm__ volatile ("dsb sy");

    uint64_t CNTFRQ_EL0 = 0;
    __asm__ volatile ("mrs %0, CNTFRQ_EL0" : "=r"(CNTFRQ_EL0));
    __asm__ volatile ("msr CNTV_TVAL_EL0, %0" : : "r"(CNTFRQ_EL0));
    __asm__ volatile ("msr CNTV_CTL_EL0, %0" : : "r"(ONE));
}
