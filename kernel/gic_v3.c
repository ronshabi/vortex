#include "gic_v3.h"
#include "aarch64.h"
#include "memory.h"
#include "printk.h"

// GIC Redistributor: Mark PEs as online
static void gicr_mark_pe_redist_as_online()
{
    volatile uint32_t current_waker = memory_read_u32(GICR_REG_WAKER);
    // Mark `ProcessorSleep` (bit 1) to be 0
    memory_write_u32(GICR_REG_WAKER, current_waker & ~(1UL << 1));

    // Poll `ChildrenAsleep` (bit 2) until it is zero
    while (1) {
        current_waker = memory_read_u32(GICR_REG_WAKER);
        if (!(current_waker & 0x4)) {
            printk("GICv3: PE is online\n");
            break;
        }
    }
}

static uint32_t get_affinity(void) {
    // mpidr_el1 has Aff3 in bits [32:40] 
    // copy that to the lower part of the register and get the unique id
    uint64_t mpidr_el1 = aarch64_get_mpidr_el1();
    uint32_t mpidr_el1_aff3 = ((mpidr_el1 >> 32U) & 0xFFU);
    uint32_t mpidr_el1_unique_id = (uint32_t)(mpidr_el1 & 0xFFFFFFFF) | ((mpidr_el1_aff3 << 24U) & 0xFFU);
    return mpidr_el1_unique_id;
}


void init_gic()
{
    printk("GICv3: Start initialization\n");

    printk("GICv3: Enable affinity rounting (ARE)\n");
    memory_write_u32(GICR_REG_CTLR, (1UL << 4) | (1UL << 5));

    printk("GICv3: Enable Group 1S, 1NS, 0 interrupts\n");
    memory_write_u32(GICR_REG_CTLR, (1UL << 4) | (1UL << 5) | 7);

    // Get Affinity
    uint32_t affinity = get_affinity();

    printk("Affinity: 0x%X\na", affinity);

    // TODO: What if the affinity is not zero
    if (affinity != 0) {
        printk("!!! AFFINITY IS NOT ZERO");
        return;
    }

    // GIC Redistributor: Mark PEs as online
    printk("GICv3: Try to mark PEs as online\n");
    gicr_mark_pe_redist_as_online();

    printk("GICv3: Set interrupt priority\n");
    aarch64_write_icc_pmr_el1(0xFFUL);

    printk("GICv3: Enable Group 0 interrupts\n");
    aarch64_write_icc_igrpen0_el1(1);

    printk("GICv3: Enable Group 1 interrupts\n");
    aarch64_write_icc_igrpen1_el1(1);

    printk("GICv3: Finished initializing\n");

}

void enable_timer(void) // int 30
{
    // Setting interrupt priority
    // ipriorityR of GICR (since this is a PPI)
    // 30 // 4 = 7, so n=7 => IPRIORITYR<7>
    // byte is 2, since 30mod4 = 2
    // we need to write 00FF0000 to IPRIORITY<7>
    // memory_write_u32(GICR_REG_IPRIORITYR(7), 0x00FF0000U);
    memory_write_u8(GICR_REG_SGI_IPRIORITYR(7) + 2, 1);

    // Setting interrupt group to group 0
    // group register is for id 30, so it is register 0
    uint32_t gicd_igroupr_0 = memory_read_u32(GICR_REG_SGI_IGROUPR(0));
    uint32_t gicd_igrpmod_0 = memory_read_u32(GICR_REG_SGI_IGRPMODR(0));

    gicd_igroupr_0 &= ~(1U << 30);
    gicd_igrpmod_0 &= ~(1U << 30);

    // update them groupz
    memory_write_u32(GICR_REG_SGI_IGROUPR(0), gicd_igroupr_0);
    memory_write_u32(GICR_REG_SGI_IGRPMODR(0), gicd_igrpmod_0);

    // Now enable int 30

    memory_write_u32(GICD_REG_ISENABLER(0), 1U << 30);
}