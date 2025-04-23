#pragma once

#include "exception_info.h"

__attribute__((noreturn)) void panic(struct exception_info *info);
