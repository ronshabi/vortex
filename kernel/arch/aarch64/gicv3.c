#include <arch/aarch64/aarch64.h>
#include <arch/aarch64/gicv3.h>
#include <common/bitops.h>
#include <memory.h>
#include <printk.h>

#define GIC_MASK_ALLOW_ALL_INTS 0xFF
#define GIC_MAX_PPI             31

// GIC Redistributor: Mark PEs as online
static void gicr_wakeup()
{
    volatile uint32_t current_waker = mem_read_u32(GICR_REG_WAKER);

    // Mark 'ProcessorSleep' (bit 1) to be 0
    mem_write_u32(GICR_REG_WAKER, current_waker & BIT_ALL_EXCEPT(1));

    // Poll 'ChildrenAsleep' (bit 2) until it is zero
    while (1)
    {
        current_waker = mem_read_u32(GICR_REG_WAKER);
        if (BIT_GET(current_waker, 2) == 0)
        {
            printk("GICv3: PE is online\n");
            break;
        }
    }
}

void gicv3_enable_sysreg_access()
{
    // enable sysreg for controlling the interrupt controller
    uint64_t ICC_SRE_EL1 = 0;
    __asm__ volatile("mrs %0, ICC_SRE_EL1" : "=r"(ICC_SRE_EL1));
    __asm__ volatile("msr ICC_SRE_EL1, %0" : : "r"(ICC_SRE_EL1 | BIT(0)));
    __asm__ volatile("isb");
}

void gicv3_set_priority_mask(uint64_t mask)
{
    __asm__ volatile("msr ICC_PMR_EL1, %0" : : "r"(mask));
}

void gicv3_enable_group_1_interrupts()
{
    // enable group1 (ns in qemu's virt) delivery
    const uint64_t ONE = 1;
    __asm__ volatile("msr ICC_IGRPEN1_EL1, %0" : : "r"(ONE));
    __asm__ volatile("isb");
}

void gicv3_enable_group_1_in_distributor()
{
    // enable the distributor
    mem_write_u32(GICD_REG_CTLR, 0x2);
    __asm__ volatile("dsb sy");
}

// Set PPI to be in group 1 (ns) and unmask it
void gicv3_enable_ppi(uint32_t ppi_number)
{
    if (ppi_number > GIC_MAX_PPI)
    {
        printk("gicv3: Error trying to enable PPI %u! (out-of-range)\n",
               ppi_number);
        return;
    }

    mem_write_u32(GICR_REG_SGI_ISENABLER0, BIT(ppi_number));
    mem_write_u32(GICR_REG_SGI_IGROUPR0,
                  mem_read_u32(GICR_REG_SGI_IGROUPR0) | BIT(ppi_number));
    printk("gicv3: Enabled PPI #%u\n", ppi_number);
}

void init_gic()
{
    printk("GICv3: Initialize\n");
    gicr_wakeup();
    gicv3_enable_ppi(27); // virtual timer PPI
    gicv3_enable_sysreg_access();
    gicv3_set_priority_mask(
        GIC_MASK_ALLOW_ALL_INTS); // 0 is the highest prio, so we are fine
    gicv3_enable_group_1_interrupts();
    gicv3_enable_group_1_in_distributor();
}
