#pragma once

#include <stdint.h>

static inline void aarch64_asm_dsb_sy(void)
{
    __asm__ volatile("dsb sy");
}

static inline uint64_t aarch64_get_mpidr_el1(void)
{
    volatile uint64_t o = 0;
    __asm__ volatile("mrs %0, MPIDR_EL1" : "=r"(o));
    return o;
}

static inline void aarch64_write_icc_pmr_el1(uint64_t value)
{
    __asm__ volatile ("msr ICC_PMR_EL1, %0" :: "r"(value));
}

static inline void aarch64_write_icc_igrpen0_el1(uint64_t value)
{
    __asm__ volatile ("msr ICC_IGRPEN0_EL1, %0" :: "r"(value));
}

static inline void aarch64_write_icc_igrpen1_el1(uint64_t value)
{
    __asm__ volatile ("msr ICC_IGRPEN1_EL1, %0" :: "r"(value));
}