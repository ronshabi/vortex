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
    "-ggdb"
    "-nostdlib"
    "-ffreestanding"
    "-fno-builtin"
    "-fno-lto"
)

set(kernel_sources
"kernel/arch/aarch64/aarch64.h"
"kernel/arch/aarch64/boot.S"
"kernel/arch/aarch64/exception_info.c"
"kernel/arch/aarch64/exception_info.h"
"kernel/arch/aarch64/exception.S"
"kernel/arch/aarch64/gicv3.c"
"kernel/arch/aarch64/gicv3.h"
"kernel/arch/aarch64/halt.S"
"kernel/arch/aarch64/irq.c"
"kernel/arch/aarch64/irq.h"
"kernel/arch/aarch64/ksyms.c"
"kernel/arch/aarch64/ksyms.h"
"kernel/arch/aarch64/panic.c"
"kernel/arch/aarch64/panic.h"
"kernel/arch/aarch64/timer.c"
"kernel/arch/aarch64/timer.h"
"kernel/common/bitops.h"
"kernel/common/stringutils.c"
"kernel/common/stringutils.h"
"kernel/debugging.c"
"kernel/debugging.h"
"kernel/devicetree/devicetree.c"
"kernel/devicetree/devicetree.h"
"kernel/drivers/pci.c"
"kernel/drivers/pci.h"
"kernel/drivers/uart.c"
"kernel/drivers/uart.h"
"kernel/drivers/virtio.c"
"kernel/drivers/virtio.h"
"kernel/klibc/byteswap.h"
"kernel/klibc/ctype.c"
"kernel/klibc/ctype.h"
"kernel/klibc/stdlib.c"
"kernel/klibc/stdlib.h"
"kernel/klibc/string.c"
"kernel/klibc/string.h"
"kernel/logbuffer.c"
"kernel/logbuffer.h"
"kernel/main.c"
"kernel/memory.c"
"kernel/memory.h"
"kernel/printk.c"
"kernel/printk.h"
"kernel/types.h"
)

add_executable(
    vortex_nosyms
    ${kernel_sources}
)

target_compile_options(vortex_nosyms PUBLIC ${cflags_cross})
target_compile_options(vortex_nosyms PUBLIC ${cflags_common})
set_target_properties(vortex_nosyms PROPERTIES LINK_DEPENDS "${CMAKE_SOURCE_DIR}/kernel/linker.ld")
set_target_properties(vortex_nosyms PROPERTIES LINKER_TYPE "LLD")

target_include_directories(vortex_nosyms PUBLIC "${CMAKE_SOURCE_DIR}/kernel")

target_link_options(vortex_nosyms PUBLIC ${cflags_cross} ${cflags_common})
target_link_options(vortex_nosyms PUBLIC "LINKER:-T" "${CMAKE_SOURCE_DIR}/kernel/linker.ld")

# Some kernel config
target_compile_definitions(vortex_nosyms PUBLIC "VORTEXLE=1") # Little Endian
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