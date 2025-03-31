#pragma once

#include <stdint.h>

#define AARCH64_GENERAL_REGISTERS 31

struct exception_context
{
    uint64_t regs[AARCH64_GENERAL_REGISTERS];
    uint64_t current_el;
    uint64_t elr_elx;
    uint64_t esr_elx;
    uint64_t spsr_elx;
} __attribute__((__aligned__(16)));

_Static_assert(sizeof(struct exception_context) % 16 == 0,
               "struct exception_context must be divisible by 16 to comply "
               "with AArch64 limitations!");