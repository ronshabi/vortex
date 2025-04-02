#!/bin/bash

if [ -z "$1" ]; then
    echo "usage: $0 [elf file] > symbols.map"
    exit 1
fi

aarch64-none-elf-nm -n "$1"