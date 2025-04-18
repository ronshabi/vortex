#include <stdint.h>

#include "ksyms.h"
#include "printk.h"

#include "logbuffer.h"

struct log_buffer  printk_log_buffer;
struct log_buffer *printk_log_buffer_ptr;

// void dummy2() {
//     __asm__("svc #0");

// }

// void dummy1() {
//     dummy2();
// }

void
func4()
{
    *(volatile int *)0x7f7f7f7f = 42; // cause a fault
}
void
func3()
{
    func4();
}

void
func2()
{
    func3();
}
void
func1()
{
    func2();
}

void
kmain(void)
{
    log_buffer_init(&printk_log_buffer);
    printk_log_buffer_ptr = &printk_log_buffer;

    printk("-*- Vortex -*-\n");


}