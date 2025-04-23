#include "timer.h"

#include <stdint.h>

#define VIRTUAL_TIMER_CTL_ENABLED 1

void virtual_timer_set_control_reg(uint32_t value);

void virtual_timer_enable()
{
    virtual_timer_set_timer_value(10 * 1000 * 1000); // 65mhz
    virtual_timer_set_control_reg(VIRTUAL_TIMER_CTL_ENABLED);
}

void virtual_timer_set_control_reg(uint32_t value)
{
    __asm__ volatile("msr CNTV_CTL_EL0, %0" : : "r"(value));
}

void virtual_timer_set_timer_value(uint64_t value)
{
    __asm__ volatile("msr CNTV_TVAL_EL0, %0" : : "r"(value));
}
