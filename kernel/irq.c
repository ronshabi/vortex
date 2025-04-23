#include "irq.h"
#include "aarch64.h"
#include "panic.h"
#include "printk.h"

#define INTID_VIRTUAL_TIMER      27
#define INTID_LEGACY_OPERATION   1022
#define INTID_SPURIOUS_INTERRUPT 1023

void irq_handler(struct exception_info *info)
{
    uint32_t intid = aarch64_get_irq_intid_and_advance();

    switch (intid)
    {
    case INTID_VIRTUAL_TIMER:
        printk("IRQ Handler: Virtual Timer\n");
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
        printk("IRQ Handler: Unknown INTID %u!\n", intid);
        panic(info);
        break;
    }
}