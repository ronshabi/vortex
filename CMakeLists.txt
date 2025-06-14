# Buildsystem for Vortex OS
# Copyright: Ron Shabi <ronsh.net>, 2025

cmake_minimum_required(VERSION 4.0)
project(Vortex C ASM)

message(STATUS "Source dir: ${CMAKE_SOURCE_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_AR_COMPILER clang)

set(cflags_cross
    "--target=aarch64-unknown-elf"
    "-mcpu=cortex-a72"
    "-mgeneral-regs-only"
    "-fno-omit-frame-pointer"
    "-fno-optimize-sibling-calls"
)

set(cflags_common
    "-Wall"
    "-Werror"
    "-Wextra"
    "-Wformat"
    "-Wconversion"
    "-std=c99"
    "-g"
    "-nostdlib"
    "-ffreestanding"
    "-fno-builtin"
    "-fno-lto"
)

set(kernel_sources
    "kernel/aarch64.h"
    "kernel/bits.h"
    "kernel/boot.S"
    "kernel/byteswap.h"
    "kernel/debugging.c"
    "kernel/debugging.h"
    "kernel/exception_info.c"
    "kernel/exception_info.h"
    "kernel/exception.S"
    "kernel/gicv3.c"
    "kernel/gicv3.h"
    "kernel/halt.S"
    "kernel/irq.c"
    "kernel/irq.h"
    "kernel/ksyms.c"
    "kernel/ksyms.h"
    "kernel/logbuffer.c"
    "kernel/logbuffer.h"
    "kernel/main.c"
    "kernel/memory.c"
    "kernel/memory.h"
    "kernel/panic.c"
    "kernel/panic.h"
    "kernel/pci.c"
    "kernel/pci.h"
    "kernel/printk.c"
    "kernel/printk.h"
    "kernel/stringutils.c"
    "kernel/stringutils.h"
    "kernel/timer.c"
    "kernel/timer.h"
    "kernel/types.h"
    "kernel/uart.c"
    "kernel/uart.h"
    "kernel/virtio.c"
    "kernel/virtio.h"
    "kernel/libc/ctype.c"
    "kernel/libc/ctype.h"
    "kernel/libc/string.h"
    "kernel/libc/stdlib.h"
    "kernel/libc/stdlib.c"
    "kernel/libc/string.c"
)

add_executable(
    vortex_nosyms
    ${kernel_sources}
)

target_compile_options(vortex_nosyms PUBLIC ${cflags_cross})
target_compile_options(vortex_nosyms PUBLIC ${cflags_common})
set_target_properties(vortex_nosyms PROPERTIES LINK_DEPENDS "${CMAKE_SOURCE_DIR}/kernel/linker.ld")
set_target_properties(vortex_nosyms PROPERTIES LINKER_TYPE "LLD")

target_link_options(vortex_nosyms PUBLIC ${cflags_cross} ${cflags_common})
target_link_options(vortex_nosyms PUBLIC "LINKER:-T" "${CMAKE_SOURCE_DIR}/kernel/linker.ld")

# Some kernel config
target_compile_definitions(vortex_nosyms PUBLIC "VORTEX_CONF_BASE_ADDRESS=0x40100000")

# Generate ksyms

add_custom_command(
    OUTPUT "symbols.map"
    COMMAND llvm-nm -nW "vortex_nosyms" > "symbols.map"
    COMMENT "Generating kernel symbols.map"
    DEPENDS "vortex_nosyms"
    CODEGEN
)

add_custom_command(
    OUTPUT "symbols.S"
    COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/gen_symbols_asm.py "symbols.map"  > "symbols.S"
    COMMENT "Generating kernel symbols.S"
    DEPENDS "symbols.map"
    CODEGEN
)

# Build phase 2
add_executable(
    vortex
    "symbols.S"
    $<TARGET_OBJECTS:vortex_nosyms>
)


target_compile_options(vortex PUBLIC ${cflags_cross} ${cflags_common})
set_target_properties(vortex PROPERTIES LINK_DEPENDS "${CMAKE_SOURCE_DIR}/kernel/linker.ld")
set_target_properties(vortex PROPERTIES LINKER_TYPE "LLD")
target_link_options(vortex PUBLIC ${cflags_cross} ${cflags_common})
target_link_options(vortex PUBLIC "LINKER:-T" "${CMAKE_SOURCE_DIR}/kernel/linker.ld")

# Run from ninja!

add_custom_target(qemu
    python3 ${CMAKE_SOURCE_DIR}/scripts/run.py "$<TARGET_FILE:vortex>"
    DEPENDS "$<TARGET_FILE:vortex>"
    COMMENT "Running QEMU"
    VERBATIM
    USES_TERMINAL
)