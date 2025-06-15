#pragma once

#include <stdint.h>


struct devicetree
{
    uint64_t base;
    
    struct {
        uint8_t size_cells;
        uint8_t address_cells;

        uint64_t strings_base;
        uint64_t strings_size;
        uint64_t structs_base;
        uint64_t structs_size;
    } fdt;
    
    uint64_t ram_base;
    uint64_t ram_size;
};

void devicetree_init(struct devicetree *dt, uint64_t base_address);
void devicetree_parse(struct devicetree *dt);