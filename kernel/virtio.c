#include "virtio.h"
#include "debugging.h"
#include "memory.h"
#include "printk.h"

#include "libc/string.h"

// Note (Ron): look at
// 4.2.3 MMIO-specific Initialization And Device Operation
// in the spec

// FIXMEs:
// - Our CPU is LE so reading values from virtio-mmio which is also LE works
//   fine, but we should wrap every read in a byte-swap function according
//   to the compile-time configuration.

#define MAGIC                   0x74726976 // ascii value of "virt", see 4.2.2.1
#define MAGIC_OFFSET            0x0000ULL
#define VERSION_OFFSET          0x0004ULL
#define LEGACY_DEVICE_ID_OFFSET 0x0008ULL // See table 4.2.4
#define LEGACY_VENDOR_ID_OFFSET 0x000CULL // See table 4.2.4

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
    uint32_t const magic = mem_read_u32((uint64_t)dev->base + MAGIC_OFFSET);
    if (magic != MAGIC)
    {
        printk("! Virtio device '%s' @ %p magic value mismatch (got 0x%X)\n",
               dev->name, dev->base, magic);
        return 1;
    }

    dev->version = mem_read_u32((uint64_t)dev->base + VERSION_OFFSET);

    // TODO: Support modern-versioned Virtio MMIO devices
    if (dev->version == VIRTIO_VERSION_MODERN)
    {
        printk("! Virtio device '%s' @ %p is modern, modern version is support "
               "is currently unimplementeded\n",
               dev->name, dev->base);
        return 1;
    }

    if (dev->version != VIRTIO_VERSION_LEGACY)
    {
        printk("! Virtio device '%s' @ %p usupported version (0x%X)\n",
               dev->name, dev->base, dev->version);
        return 1;
    }

    // Read device ID and vendor ID
    dev->device_id =
        mem_read_u32((uint64_t)dev->base + LEGACY_DEVICE_ID_OFFSET);

    // 4.2.3.1.1 says we must verify DeviceID is not zero.
    //           If it is, we abort initialization and MUST NOT access any
    //           other register.

    // Also:
    // Value zero (0x0) is used to define a system memory map with
    // placeholder devices at static, well known addresses, assigning
    // functions to them depending on user’s needs.

    // See this:
    // https://stackoverflow.com/questions/76219021/who-will-configure-virtio-mmio-device-register-in-qemu
    // https://www.qemu.org/docs/master/devel/virtio-backends.html
    // https://www.qemu.org/docs/master/system/devices/usb.html

    if (dev->device_id == 0)
    {
        printk("# Virtio device '%s' @ %p device_id is zero\n", dev->name,
               dev->base);
    }

    dev->vendor_id =
        mem_read_u32((uint64_t)dev->base + LEGACY_VENDOR_ID_OFFSET);

    printk("Vendor id = %X\n", dev->vendor_id);

    printk("-> Virtio: ok so far... <-");
    return 0;
}
