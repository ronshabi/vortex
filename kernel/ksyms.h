#pragma once

#include <stdint.h>

void ksyms_print_all_symbols();

const char *ksyms_addr2line(uint64_t addr);