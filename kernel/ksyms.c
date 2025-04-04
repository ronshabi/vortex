#include "ksyms.h"

#include "printk.h"
#include "config.h"

#include <stdint.h>
#include <stddef.h>

struct ksym_entry {
    uint64_t addr;
    uint64_t string_offset;
} __attribute__((aligned(8), packed));

extern const char vortex_symbols_stringtbl[] __attribute__((section(".rodata"), weak));
extern struct ksym_entry vortex_symbols[] __attribute__((section(".rodata"), weak));

extern uint64_t vortex_symbols_stringtbl_size __attribute__((section(".rodata"), weak));
extern uint64_t vortex_symbols_size __attribute__((section(".rodata"), weak));

void ksyms_print_all_symbols()
{
    const uint64_t amount_of_symbols = vortex_symbols_size / sizeof(struct ksym_entry);

    for (uint64_t i = 0; i < amount_of_symbols; ++i) {
        const struct ksym_entry sym = vortex_symbols[i];
        printk("[0x%lX, %s]\n", sym.addr + CONF_BASEADDR, vortex_symbols_stringtbl + sym.string_offset);
    }
}

// const char *ksyms_addr2line(uint64_t addr)
// {
//     uint64_t const base = CONF_BASEADDR; 
//     if (addr < base) {
//         return NULL;
//     }

//     uint64_t const search_for = addr - base;

// }

// const char *ksyms_get_string_at_offset(uint64_t offset)
// {
//     printk("sizeof symbols: %lu\n", vortex_symbols_size);
//     printk("sizeof string table: %lu\n", vortex_symbols_stringtbl_size);
//     return NULL;
// }