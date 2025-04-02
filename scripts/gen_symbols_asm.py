#!/bin/python3

import sys

# Usage: gen_symbols_asm.py [nm output] > symbols.S

EXCLUDED_SYMBOL_TYPES = "aNnU?"
LABEL_SYMBOL_SECTION = "vortex_symbols"
LABEL_SYMBOL_SECTION_STRINGTABLE = "vortex_symbols_stringtbl"


def parse_nm_output(filename: str):
    symbols = []
    min_addr = sys.maxsize

    with open(filename, 'r') as f:
        lines = f.read().splitlines()

        for line in lines:
            addr, sym_type, func_name = line.split()

            addr_int = int(addr, 16)

            # exclude some symbol types
            if sym_type in EXCLUDED_SYMBOL_TYPES:
                continue


            if addr_int < min_addr:
                min_addr = addr_int

            symbols.append((addr_int, sym_type, func_name))

    return symbols, min_addr


def gen_asm_header(header_name, align, header_type: str):
    print(".align", align)
    print(".global", header_name)
    print(f".type {header_name}, {header_type}")
    print(f"\n{header_name}:")

def gen_asm(filename: str):
    symbols, min_addr = parse_nm_output(filename)

    symbol_name_offset = 0

    gen_asm_header(LABEL_SYMBOL_SECTION, 8, "@object")
    print("//\n// symbol address, string table offset, real address, name, type\n//")
    
    for symbol in symbols:
        symbol_addr, symbol_type, symbol_name = symbol
        symbol_addr_relative = symbol_addr - min_addr
        print(f".long 0x{symbol_addr_relative:08x}; .long {symbol_name_offset};\t// 0x{symbol_addr:016x} {symbol_name} ({symbol_type})")

        symbol_name_offset += len(symbol_name) + 1


    print()
    gen_asm_header(LABEL_SYMBOL_SECTION_STRINGTABLE, 8, "@object")


    # Symbol strings
    symbol_name_offset = 0

    for symbol in symbols:
        symbol_addr, symbol_type, symbol_name = symbol
        symbol_name = f'"{symbol_name}"'
        print(f'.asciz {symbol_name:<32}; // @{symbol_name_offset}')
        symbol_name_offset += len(symbol_name) + 1



def main():
    if len(sys.argv) != 2:
        print(f"usage: {sys.argv[0]} [nm output] > symbols.S")
        exit(1)

    try:
        gen_asm(sys.argv[1])
    except Exception as e:
        print(f"Error: failed generating symbols for {sys.argv[1]}")
        print(e)

if __name__ == "__main__":
    main()