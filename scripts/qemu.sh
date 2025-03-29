#!/bin/sh

# https://www.qemu.org/docs/master/system/generic-loader.html
# https://www.qemu.org/docs/master/system/arm/cpu-features.html
# https://www.qemu.org/docs/master/system/arm/virt.html
# https://www.qemu.org/docs/master/system/arm/emulation.html

KERNEL="kernel/kernel.elf"
LOADER="loader,file=${KERNEL},addr=0x40100000,cpu-num=0"
DEBUG=int,cpu_reset,strace,unimp

qemu-system-aarch64 -machine virt \
                    -cpu cortex-a72 \
                    -m 128m \
                    -serial stdio \
                    -device "${LOADER}" \
                    -d "${DEBUG}"
                    
