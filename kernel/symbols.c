#include "symbols.h"

#include "printk.h"

#include <stdint.h>

extern const char vortex_symbols_stringtbl[] __attribute__((section(".rodata"), weak));

void print_kernel_symbols() {
    printk("KSYM: (%lX)\n", (uint64_t)vortex_symbols_stringtbl);
}