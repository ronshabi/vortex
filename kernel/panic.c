#include "panic.h"
#include "ksyms.h"
#include "printk.h"

// Get 'halt' from asm world
extern void __attribute__((noreturn)) halt(void);

#define ANSI_BOLD     "\033[1m"
#define ANSI_BOLD_RED "\033[1;31m"
#define ANSI_CYAN     "\033[36m"
#define ANSI_RESET    "\033[0m"

// If the jump between this frame pointer and the next frame pointer
// is bigger than this number, we probably got outside of the stack.
// So, stop unwinding.
#define STOP_UNWIND_MAX_DISTANCE 0x100

__attribute__((noreturn)) void panic(struct exception_info *info)
{

    printk(ANSI_BOLD_RED "Vortex: Kernel Panic" ANSI_RESET "\n");

    printk(ANSI_CYAN "General Purpose Registers:" ANSI_RESET "\n");
    for (unsigned r = 0; r < AARCH64_GENERAL_REGISTERS; ++r)
    {
        printk(ANSI_BOLD "x%02d" ANSI_RESET ": 0x%016lX  ", r, info->regs[r]);

        if ((r + 1) % 4 == 0)
        {
            printk("\n");
        }
    }

    printk("\n\n");
    printk(ANSI_CYAN "System Registers:" ANSI_RESET "\n");
    printk(ANSI_BOLD "%10s" ANSI_RESET ": %lu\n", "CurrentEL",
           info->current_el >> 2);
    printk(ANSI_BOLD "%10s" ANSI_RESET ": 0x%lX\n", "SPSR_ELx", info->spsr_elx);
    printk(ANSI_BOLD "%10s" ANSI_RESET ": 0x%lX\n", "ELR_ELx", info->elr_elx);
    printk(ANSI_BOLD "%10s" ANSI_RESET ": 0x%lX\n", "ESR_ELx", info->esr_elx);
    printk(ANSI_BOLD "%10s" ANSI_RESET ": 0x%lX\n", "FAR_ELx", info->far_elx);

    printk("\n");
    printk(ANSI_CYAN "Call Stack:" ANSI_RESET "\n");

    // unwind
    volatile uint64_t *fp =
        (uint64_t *)info->regs[AARCH64_FRAME_POINTER_REGISTER];
    volatile uint64_t *next_fp   = fp;
    int                depth     = 0;
    const int          max_depth = 7;

    uint64_t lr = info->regs[30];

    while (depth < max_depth && fp)
    {
        const char *symbol_name = ksyms_addr2line(lr);
        printk("    [%d] <0x%lX> %s\n", depth, lr, symbol_name);
        lr      = fp[1];
        next_fp = (uint64_t *)*fp; // next frame please

        if ((fp - next_fp) > STOP_UNWIND_MAX_DISTANCE)
        {
            break;
        }

        fp = next_fp;
        ++depth;
    }

    halt();
}
