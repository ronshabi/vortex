#pragma once

#include <stdint.h>
#include <stddef.h>

void memory_write_u32(uint64_t address, uint32_t value);
void memory_write_u8(uint64_t address, uint8_t value);
uint32_t memory_read_u32(uint64_t address);
uint32_t memory_read_u32_bswap(uint64_t address);