#include "types.h"

void printk(const char *s)
{
    volatile uint8_t *uart = (uint8_t *)0x09000000;
    while (*s) {
        *uart = *((uint8_t*)s);
        ++s;
    }
}

void kmain(void)
{
    printk("hello world");
    
    while (1) {

    }
}