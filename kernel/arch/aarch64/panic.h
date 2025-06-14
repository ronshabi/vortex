#pragma once

#include <arch/aarch64/exception_info.h>

__attribute__((noreturn)) void panic(struct exception_info *info);
