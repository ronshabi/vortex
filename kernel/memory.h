#pragma once

#include <stddef.h>
#include <stdint.h>
#include <klibc/endian.h>

static inline void mem_write_u8(uint64_t address, uint8_t value)
{
    *((volatile uint8_t *)address) = value;
}

static inline void mem_write_u16(uint64_t address, uint16_t value)
{
    *((volatile uint16_t *)address) = value;
}

static inline void mem_write_u32(uint64_t address, uint32_t value)
{
    *((volatile uint32_t *)address) = value;
}

static inline void mem_write_u64(uint64_t address, uint64_t value)
{
    *((volatile uint64_t *)address) = value;
}

static inline uint8_t mem_read_u8(uint64_t address)
{
    return *((volatile uint8_t *)address);
}

static inline uint16_t mem_read_u16(uint64_t address)
{
    return *((volatile uint16_t *)address);
}

static inline uint32_t mem_read_u32(uint64_t address)
{
    return *((volatile uint32_t *)address);
}

static inline uint32_t mem_read_u32be(uint64_t address)
{
    return be32toh(*((volatile uint32_t *)address));
}

static inline uint64_t mem_read_u64(uint64_t address)
{
    return *((volatile uint64_t *)address);
}

static inline uint64_t mem_read_u64be(uint64_t address)
{
    return be64toh(*((volatile uint64_t *)address));
}
