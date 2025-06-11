#include "debugging.h"
#include "memory.h"
#include "printk.h"
#include <stdint.h>

void hexdump(uint8_t *ptr, uint64_t size)
{
    uint64_t       i             = 0;
    const uint64_t words_in_line = 4;

    printk("--- hexdump of %p <%lu> ---\n", ptr, size);

    for (i = 0; i < size; i++)
    {
        printk("%016lX ", mem_read_u64((uint64_t)((uint64_t *)ptr + i)));

        if ((i + 1) % words_in_line == 0)
        {
            printk("\n");
        }
    }
}
