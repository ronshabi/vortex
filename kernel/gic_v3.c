#include "gic_v3.h"
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

    printk("GICv3: Enable affinity rounting (ARE)\n");
    mem_write_u32(GICR_REG_CTLR, (BIT(4) | BIT(5)));

    printk("GICv3: Enable Group 1S, 1NS, 0 interrupts\n");
    mem_write_u32(GICR_REG_CTLR, (BIT(4) | BIT(5) | 7));

    // GIC Redistributor: Mark PEs as online
    printk("GICv3: Try to wakeup redistributor\n");
    gicr_wakeup();

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
    mem_write_u8(GICR_REG_SGI_IPRIORITYR(7) + 2, 1);

    // Setting interrupt group to group 0
    // group register is for id 30, so it is register 0
    uint32_t gicd_igroupr_0 = mem_read_u32(GICR_REG_SGI_IGROUPR(0));
    uint32_t gicd_igrpmod_0 = mem_read_u32(GICR_REG_SGI_IGRPMODR(0));

    gicd_igroupr_0 &= BIT_ALL_EXCEPT(30);
    gicd_igrpmod_0 &= BIT_ALL_EXCEPT(30);

    // update them groupz
    mem_write_u32(GICR_REG_SGI_IGROUPR(0), gicd_igroupr_0);
    mem_write_u32(GICR_REG_SGI_IGRPMODR(0), gicd_igrpmod_0);

    // Now enable int 30
    mem_write_u32(GICR_REG_ISENABLER0, BIT(30));

    printk("VT: Set ticks\n");

    aarch64_set_virtual_timer_ticks(1000000);
    aarch64_set_virtal_timer_state(1);

    printk("After\n");
}