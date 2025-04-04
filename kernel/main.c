#include <stdint.h>

#include "printk.h"
#include "ksyms.h"

#include "logbuffer.h"

struct log_buffer  printk_log_buffer;
struct log_buffer *printk_log_buffer_ptr;

// void dummy2() {
//     __asm__("svc #0");

// }

// void dummy1() {
//     dummy2();
// }



void func4() {
    *(volatile int *)0x7f7f7f7f = 42; // cause a fault
    
}
void func3() {
    func4();
}

void func2() { func3(); }
void func1() { func2(); }



void
kmain(void)
{
    log_buffer_init(&printk_log_buffer);
    printk_log_buffer_ptr = &printk_log_buffer;

    printk("hello world %x\n", 0xdeadbeef);

    ksyms_print_all_symbols();

    // do some funny shit

    // volatile int *x = (int *)0x7F7F7F7F;
    // *x              = 20;
    // func1();
}