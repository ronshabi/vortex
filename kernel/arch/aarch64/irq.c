#include <arch/aarch64/aarch64.h>
#include <arch/aarch64/irq.h>
#include <arch/aarch64/panic.h>
#include <arch/aarch64/timer.h>
#include <printk.h>

#define INTID_VIRTUAL_TIMER      27
#define INTID_LEGACY_OPERATION   1022
#define INTID_SPURIOUS_INTERRUPT 1023

static uint64_t retry = 0;

void irq_handler(struct exception_info *info)
{
    uint64_t intid = aarch64_get_irq_intid_and_advance();

    switch (intid)
    {
    case INTID_VIRTUAL_TIMER:
        printk("IRQ Handler: Virtual Timer\n");
        virtual_timer_set_timer_value(40 * 1000 * 1000);

        uint64_t sp = 0;
        __asm__ volatile("mov %0, sp" : "=r"(sp));

        printk("+ Returning (%lu), SP = 0x%lX\n", retry++, sp);
        __asm__ volatile("msr ICC_EOIR1_EL1, %0" ::"r"(intid));
        break;
    case INTID_LEGACY_OPERATION:
        printk("IRQ Handler: Legacy Operation!\n");
        panic(info);
        break;
    case INTID_SPURIOUS_INTERRUPT:
        printk("IRQ Handler: Spurious interrupt!\n");
        panic(info);
        break;
    default:
        printk("IRQ Handler: Unknown INTID %lu!\n", intid);
        panic(info);
        break;
    }
}