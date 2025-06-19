#!/usr/bin/python3

import os
import subprocess
import argparse

class Qemu:
    def __init__(self, kernel_path):
        self.executable = "qemu-system-aarch64"
        self.machine = "virt"
        self.cpu = "cortex-a72"
        self.mem = "128m"
        self.gic_version = 3

        self.kernel = kernel_path
        self.kernel_load_address = 0x40100000
        self.kernel_load_cpu_num = 0

        # https://wiki.debian.org/Arm64Qemu
        # Debian for Arm64 uses usb-ehci, so we will use it instead of XHCI

        self.devices = [
            f"loader,file={self.kernel},addr={self.kernel_load_address:X},cpu-num={self.kernel_load_cpu_num}",
            "usb-ehci",
            "usb-kbd",
            "virtio-gpu-pci",
        ]

        self.additional_args = ["-nographic"]

        self.tracing_opts = [
            "gicv3_*",
            "virtio_*",
            "acpi_*",
            "pci_*",
            "qemu_vfio*",
            "vfio_*",
            "dma_*",
            "cpu_*",
            "usb_*",
            "uefi_*",
            "arm_*",
            "clock_*",
            "system_wakeup_request",
        ]

        self.debug_messages = [
            "int",
            "cpu_reset",
            "strace",
            "unimp",
            "guest_errors",
            "strace",
        ]

        self.debug = False


    def set_kernel(self, path):
        self.kernel = path

    def get_command(self):
        cmd = [self.executable]

        # Add all additional arguments
        cmd.extend(self.additional_args)

        # Add machine
        machine_string = f"{self.machine},gic-version={self.gic_version}"
        cmd.extend(["-machine", machine_string])

        # Add CPU
        cmd.extend(["-cpu", self.cpu])

        # Add memory
        cmd.extend(["-m", self.mem])

        # Add all devices
        for device in self.devices:
            cmd.extend(["-device", device])

        # Add all debug messages
        cmd.extend(["-d", ",".join(self.debug_messages)])

        # Add all tracing messages
        for tracing_opt in self.tracing_opts:
            cmd.extend(["--trace", tracing_opt])

        if self.debug:
            cmd.extend(["-S", "-s"])

        return cmd

    def set_debugging(self, to):
        self.debug = to

    def run(self):
        # Bye python world
        os.execvp(self.executable, self.get_command())


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("kernel", help="Kernel ELF to execute")
    parser.add_argument(
        "--debug",
        "-d",
        help="Enable GDB through QEMU -S -s",
        action="store_true",
        default=False,
        required=False,
    )

    args = parser.parse_args()

    print("IS DEBUGGING?", args.debug)

    qemu = Qemu(args.kernel)
    qemu.set_debugging(args.debug)
    qemu.run()


if __name__ == "__main__":
    main()
