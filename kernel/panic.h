#pragma once

#include "exception_context.h"

__attribute__((noreturn)) void panic(struct exception_context *context);