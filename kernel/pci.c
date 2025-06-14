#include "pci.h"
#include "memory.h"
#include "printk.h"
#include <stdint.h>

static uint32_t readreg(uint64_t offset)
{
    const uint64_t addr  = (uint64_t)(PCI_BASE + offset);
    const uint32_t value = mem_read_u32((uint64_t)addr);
    printk("[  PCI] R[0x%08lx]: 0x%08x\n", addr, value);
    return value;
}

// static void writereg(uint64_t offset, uint32_t val)
// {
//     const uint64_t addr = (uint64_t)(PCI_BASE + offset);
//     mem_write_u32(addr, val);
//     printk("[  PCI] W[0x%08lx]: 0x%08x\n", addr, val);
// }

int pci_init()
{
    printk("pci: init\n");

    for (uint64_t i = 0; i < 0x00010000; i += 4)
    {
        // writereg(i, 0);
        readreg(i);
    }

    return 0;
}
