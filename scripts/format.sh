#!/bin/bash

find 'kernel' -type f -name '*.[ch]' -exec clang-format --verbose -i '{}' '+'