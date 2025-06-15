#pragma once

// #include "types.h"
#include <stdint.h>

void uart_init(void);
void uart_write(const char *buf);
void uart_write_ch(int ch);