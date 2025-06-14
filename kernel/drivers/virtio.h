#pragma once

// Modeled after this spec
// https://docs.oasis-open.org/virtio/virtio/v1.1/cs01/virtio-v1.1-cs01.html
// ~ Ron, 11.06.2025

#include <stdint.h>

// Not in the standard, this is for associating devices with string names
#define VIRTIO_MMIO_DEVICE_NAME_LENGTH 64

// Standard table 4.1
#define VIRTIO_VERSION_MODERN 0x2
#define VIRTIO_VERSION_LEGACY 0x1

#define VIRTIO_VENDOR_ID_QEMU 0x554D4551UL

#define VIRTIO_DEVICE_STATUS_ACKNOWLEDGE        1
#define VIRTIO_DEVICE_STATUS_DRIVER             2
#define VIRTIO_DEVICE_STATUS_DRIVER_OK          4
#define VIRTIO_DEVICE_STATUS_FEATURES_OK        8
#define VIRTIO_DEVICE_STATUS_DEVICE_NEEDS_RESET 64
#define VIRTIO_DEVICE_STATUS_FAILED             128

struct virtio_mmio_device
{
    char name[VIRTIO_MMIO_DEVICE_NAME_LENGTH];

    uint8_t *base;

    uint32_t version;
    uint32_t device_id;
    uint32_t vendor_id;
    uint32_t feature_bits;
};

int init_virtio_mmio_device(struct virtio_mmio_device *dev, const char *name,
                            uint8_t *base_address);