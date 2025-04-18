#pragma once

#include <stdint.h>
#include <stddef.h>

uint32_t memory_read_u32_le(uint64_t address);
uint32_t memory_read_u32_be(uint64_t address);