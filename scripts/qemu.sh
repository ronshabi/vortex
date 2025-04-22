#!/bin/sh

# https://www.qemu.org/docs/master/system/generic-loader.html
# https://www.qemu.org/docs/master/system/arm/cpu-features.html
# https://www.qemu.org/docs/master/system/arm/virt.html
# https://www.qemu.org/docs/master/system/arm/emulation.html

set -x

KERNEL="$1"
LOADER="loader,file=${KERNEL},addr=0x40100000,cpu-num=0"
DEBUG=int,cpu_reset,strace,unimp,guest_errors,strace
TRACING="gicv3_* virtio_mmio_* virtio_iommu*" #pl011_* 

TRACING_OPTS=""

for word in $TRACING; do
    TRACING_OPTS+="--trace $word " 
done

qemu-system-aarch64 -machine virt,gic-version=3 \
                    -cpu cortex-a72 \
                    -m 128m \
                    -device "${LOADER}" -device usb-ehci --device usb-kbd \
                    -d "${DEBUG}" \
                    -nographic \
                    $TRACING_OPTS


