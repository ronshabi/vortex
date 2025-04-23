#pragma once

#include <stdint.h>

static inline uint32_t aarch64_get_irq_intid_and_advance(void)
{
    uint64_t icc_iar1_el1 = 0;
    __asm__ volatile("mrs %0, ICC_IAR1_EL1" : "=r"(icc_iar1_el1));
    return (uint32_t)icc_iar1_el1;
}