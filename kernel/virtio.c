#include "virtio.h"
#include "debugging.h"
#include "klibc.h"
#include "memory.h"
#include "printk.h"

// Note (Ron): look at
// 4.2.3 MMIO-specific Initialization And Device Operation
// in the spec

int init_virtio_mmio_device(struct virtio_mmio_device *dev, const char *name,
                            uint8_t *base_address)
{
    if (strlen(name) >= VIRTIO_MMIO_DEVICE_NAME_LENGTH)
    {
        printk("! Virtio device name '%s' is too large\n", name);
        return 1;
    }

    // This is not in the standard, I just want to keep things organized
    memset8((uint8_t *)dev, 0, sizeof(struct virtio_mmio_device));
    printk("Trying to initialize virtio device '%s'\n", name);
    strcpy(dev->name, name);
    dev->base = base_address;

    // 4.2.3.1.1 -> Read and check values MagicValue and Version
    uint32_t const magic = mem_read_u32((uint64_t)dev->base);
    if (magic != VIRTIO_MAGIC_VALUE)
    {
        printk("! Virtio device '%s' @ %p magic value mismatch (got 0x%X)",
               dev->name, dev->base, magic);
        return 1;
    }

    printk("-> Virtio: ok so far... <-");
}
