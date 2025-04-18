#include "memory.h"
#include "byteswap.h"

uint32_t memory_read_u32_le(uint64_t address)
{
    return *(volatile uint32_t *)address;
}

uint32_t memory_read_u32_be(uint64_t address)
{
    return bswap_32(memory_read_u32_le(address));
}