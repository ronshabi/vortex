#pragma once

__attribute__((format(printf, 1, 2))) void
printk(const char *fmt, ...);