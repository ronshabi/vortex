#include "irq.h"
#include "printk.h"

void irq_handler()
{
    printk("irq yo\n");
}