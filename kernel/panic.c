#include "panic.h"
#include "printk.h"

// Get 'halt' from asm world
extern void __attribute__((noreturn))
halt(void);

#define ANSI_BOLD "\033[1m"
#define ANSI_BOLD_RED "\033[1;31m"
#define ANSI_CYAN "\033[36m"
#define ANSI_RESET "\033[0m"



__attribute__((noreturn)) void
panic(struct exception_info *info)
{
    printk(ANSI_BOLD_RED "Vortex: Kernel Panic" ANSI_RESET "\n");

    printk(ANSI_CYAN "General Purpose Registers:" ANSI_RESET "\n");
    for (unsigned r = 0; r < AARCH64_GENERAL_REGISTERS; ++r) {
        printk(ANSI_BOLD "0x%02d" ANSI_RESET ": 0x%08lX\t", r, info->regs[r]);

        if ((r+1) % 4 == 0) {
            printk("\n");
        }
    }
    printk("\n\n");

    printk(ANSI_CYAN "System Registers:" ANSI_RESET "\n");
    printk(ANSI_BOLD "%10s" ANSI_RESET ": %lu\n", "CurrentEL", info->current_el >> 2);
    printk(ANSI_BOLD "%10s" ANSI_RESET ": 0x%lX\n", "SPSR_ELx", info->spsr_elx);
    printk(ANSI_BOLD "%10s" ANSI_RESET ": 0x%lX\n", "ELR_ELx", info->elr_elx);
    printk(ANSI_BOLD "%10s" ANSI_RESET ": 0x%lX\n", "ESR_ELx", info->esr_elx);

    printk("\n");

    halt();
}
