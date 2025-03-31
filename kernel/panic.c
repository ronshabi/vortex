#include "panic.h"
#include "printk.h"

// Get 'halt' from asm world
extern void __attribute__((noreturn)) halt(void);

__attribute__((noreturn)) void panic(struct exception_context *context)
{
    (void)context; // TODO: use the context
    printk("panic!");
    halt();
}
