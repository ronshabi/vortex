#include "ksyms.h"

#include "printk.h"

#include <stddef.h>
#include <stdint.h>

struct ksym_entry
{
    uint64_t addr;
    uint64_t string_offset;
} __attribute__((aligned(8), packed));

extern const char vortex_symbols_stringtbl[]
    __attribute__((section(".rodata"), weak));
extern struct ksym_entry vortex_symbols[]
    __attribute__((section(".rodata"), weak));

extern uint64_t vortex_symbols_stringtbl_size
    __attribute__((section(".rodata"), weak));
extern uint64_t vortex_symbols_size __attribute__((section(".rodata"), weak));

// for debugging purposes only
void ksyms_print_all_symbols()
{
    const uint64_t amount_of_symbols =
        vortex_symbols_size / sizeof(struct ksym_entry);

    for (uint64_t i = 0; i < amount_of_symbols; ++i)
    {
        const struct ksym_entry sym = vortex_symbols[i];
        printk("[0x%lX, %s]\n", sym.addr + VORTEX_CONF_BASE_ADDRESS,
               vortex_symbols_stringtbl + sym.string_offset);
    }
}

const char *ksyms_addr2line(uint64_t addr)
{
    const uint64_t amount_of_symbols =
        vortex_symbols_size / sizeof(struct ksym_entry);

    for (uint64_t i = 0; i + 1 < amount_of_symbols; ++i)
    {
        const struct ksym_entry sym      = vortex_symbols[i];
        const struct ksym_entry sym_next = vortex_symbols[i + 1];

        if (sym_next.addr + VORTEX_CONF_BASE_ADDRESS >= addr)
        {
            return vortex_symbols_stringtbl + sym.string_offset;
        }
    }

    return "??";
}