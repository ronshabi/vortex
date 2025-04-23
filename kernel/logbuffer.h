#pragma once

#define LOG_BUFFER_SIZE 1024

#include <stdbool.h>
#include <stddef.h>

struct log_buffer
{
    char   buffer[LOG_BUFFER_SIZE];
    size_t head;
    size_t tail;
};

void log_buffer_init(struct log_buffer *lb);
int  log_buffer_writech(struct log_buffer *lb, char c);
int  log_buffer_writech_repeating(struct log_buffer *lb, char c, size_t n);
int  log_buffer_write(struct log_buffer *lb, const char *str);
int  log_buffer_readch(struct log_buffer *lb);
void log_buffer_flush(struct log_buffer *lb, void (*outfunc)(int c));