#include "memory.h"
#include "byteswap.h"

uint32_t memory_read_u32(uint64_t address)
{
    return *(volatile uint32_t *)address;
}


uint32_t memory_read_u32_bswap(uint64_t address)
{
    return bswap_32(memory_read_u32(address));
}

void memory_write_u32(uint64_t address, uint32_t value)
{
    *((volatile uint32_t*)address) = value;
}

void memory_write_u8(uint64_t address, uint8_t value)
{
    *((volatile uint8_t*)address) = value;
}
