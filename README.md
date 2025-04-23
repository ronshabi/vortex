# Vortex - an ARMv8-A Operating System

Coded from scratch by Ron Shabi, mainly to learn the architecture.

## Features
- Globally usable, buffered `printk`
- Exception handler
- KSyms with an `addr2line`-like interface and unwind support
- Virtual timer IRQ

## Building
### Supported Compilers
The Vortex kernel currently compiles with GCC 14.2 for the `aarch64-none-elf` triplet. Other compilers were not tested, although will probably work, since the code is fairly simple and doesn't use any weird C extensions.

### Virtual Machine
The only tested boad is QEMU's `virt` for the Cortex-A72. I currently don't plan on implementing more boards/configurations.

See `scripts/qemu.sh` for specific configuration.

### How do I build?
Just `make` :)

Running `make qemu` will compile and run the kernel in QEMU

Note that the build is an out of tree build, such that all objects files, generated code and ELFs reside in `build` which is created by Make.


## Author
Ron Shabi <<ron@ronsh.net>>