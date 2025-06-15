#include "pci.h"
#include <memory.h>
#include <printk.h>
#include <stdint.h>

#define PCI_BASE 0x3eff0000


static uint32_t readreg(uint64_t offset)
{
    const uint64_t addr  = (uint64_t)(offset);
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

static uint64_t phys(uint64_t bus, uint64_t dev, uint64_t func) 
{
    return PCI_BASE + ((bus << 20) | (dev << 15) | (func << 12));
}

int pci_init()
{
    printk("[  PCI] init\n");
    uint64_t addr = phys(0, 0, 0);

    readreg(addr);

    return 0;
}
