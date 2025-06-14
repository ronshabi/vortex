#!/usr/bin/python3

import os
import subprocess
import sys

if len(sys.argv) != 2:
    print("usage: ./run.py [path to kernel]")
    exit(1)

KERNEL = sys.argv[1]
KERNEL_LOAD_ADDR = 0x40100000
KERNEL_LOAD_CPU_NUM = 0

QEMU = "qemu-system-aarch64"
QEMU_MACHINE = "virt"
QEMU_GIC_VERSION = 3
QEMU_CPU = "cortex-a72"
QEMU_MEMORY = "128m"

QEMU_DEVICES = []

QEMU_TRACING_OPTS = [
    "gicv3_*",
    "virtio_*",
    "virtio_iommu*",
    "acpi_*",
    "pci_*",
    "qemu_vfio*",
    "vfio_*",
]

QEMU_DEBUG_MESSAGES = ["int", "cpu_reset", "strace", "unimp", "guest_errors", "strace"]

QEMU_ADDITIONAL_ARGUMENTS = ["-nographic"]

# Add loader
QEMU_DEVICES.append(
    f"loader,file={KERNEL},addr={KERNEL_LOAD_ADDR:X},cpu-num={KERNEL_LOAD_CPU_NUM}"
)

# https://wiki.debian.org/Arm64Qemu
# Debian for Arm64 uses usb-ehci, so we will use it instead of XHCI
QEMU_DEVICES.append("usb-ehci")
QEMU_DEVICES.append("usb-kbd")


def build_command(dump_dtb_file_name=None):
    cmd = [QEMU]

    # Add all additional arguments
    cmd.extend(QEMU_ADDITIONAL_ARGUMENTS)

    # Add machine
    machine_string = f"{QEMU_MACHINE},gic-version={QEMU_GIC_VERSION}"

    if dump_dtb_file_name:
        machine_string += f",dumpdtb={dump_dtb_file_name}"

    cmd.extend(["-machine", machine_string])

    # Add CPU
    cmd.extend(["-cpu", QEMU_CPU])

    # Add memory
    cmd.extend(["-m", QEMU_MEMORY])

    # Add all devices
    for device in QEMU_DEVICES:
        cmd.extend(["-device", device])

    # Add all debug messages
    cmd.extend(["-d", ",".join(QEMU_DEBUG_MESSAGES)])

    # Add all tracing messages
    for tracing_opt in QEMU_TRACING_OPTS:
        cmd.extend(["--trace", tracing_opt])

    return cmd


cmd = build_command()


def dump_dtb(dtb_file_name: str):
    cmd_dumpdtb = build_command(dump_dtb_file_name=dtb_file_name)
    p = subprocess.run(cmd_dumpdtb)
    p.check_returncode()


def dtb_to_dts(dtb_file_name: str):
    cmd_dtc = [
        "dtc",
        "-I",
        "dtb",
        "-O",
        "dts",
        "-o",
        dtb_file_name + ".dts",
        dtb_file_name,
    ]

    p = subprocess.run(cmd_dtc)
    p.check_returncode()


def run_qemu():
    # Bye python world
    os.execvp(QEMU, cmd)


if __name__ == "__main__":
    dump_dtb("qemu.dtb")
    dtb_to_dts("qemu.dtb")
    run_qemu()
