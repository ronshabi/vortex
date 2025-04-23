#include "gicv3.h"
#include "aarch64.h"
#include "memory.h"
#include "printk.h"
#include "bits.h"

#define GIC_MASK_ALLOW_ALL_INTS 0xFF

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

void gicv3_enable_sysreg_access()
{
    // enable sysreg for controlling the interrupt controller 
    uint64_t ICC_SRE_EL1 = 0;
    __asm__ volatile ("mrs %0, ICC_SRE_EL1" : "=r"(ICC_SRE_EL1));
    __asm__ volatile ("msr ICC_SRE_EL1, %0" : : "r"(ICC_SRE_EL1 | BIT(0)));
    __asm__ volatile ("isb");
}

void gicv3_set_priority_mask(uint64_t mask)
{
    __asm__ volatile ("msr ICC_PMR_EL1, %0" : : "r"(mask));
}

void gicv3_enable_group_1_interrupts()
{
    // enable group1 (ns in qemu's virt) delivery
    const uint32_t ONE = 1;
    __asm__ volatile ("msr ICC_IGRPEN1_EL1, %0" : : "r"(ONE));
    __asm__ volatile ("isb");
}

void gicv3_enable_group_1_in_distributor()
{
    // enable the distributor
    mem_write_u32(GICD_REG_CTLR, 0x2);
    __asm__ volatile ("dsb sy");
}

void init_gic()
{
    printk("GICv3: Start initialization\n");
    gicr_wakeup();

    // set virtual timer ppi (IRQ 27) to be in group1 (ns) and unmask it
    mem_write_u32(GICR_REG_SGI_ISENABLER0, BIT(27));
    mem_write_u32(GICR_REG_SGI_IGROUPR0, mem_read_u32(GICR_REG_SGI_IGROUPR0) | BIT(27));

    gicv3_enable_sysreg_access();
    gicv3_set_priority_mask(GIC_MASK_ALLOW_ALL_INTS); // 0 is the highest prio, so we are fine
    gicv3_enable_group_1_interrupts();
    gicv3_enable_group_1_in_distributor();
}
