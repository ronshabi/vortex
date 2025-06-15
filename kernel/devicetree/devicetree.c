#include "devicetree.h"
#include "klibc/endian.h"
#include "memory.h"
#include "printk.h"

#include <klibc/string.h>

#define MAGIC 0xD00DFEEDUL

#define OFFSET_MAGIC 0
#define OFFSET_VERSION  (5 * 4)

#define OFFSET_RESERVED_BLOCK_OFF  (4 * 4)
#define OFFSET_STRUCTS_BLOCK_OFF   (2 * 4)
#define OFFSET_STRUCTS_BLOCK_SIZE  (9 * 4)
#define OFFSET_STRINGS_BLOCK_OFF   (3 * 4)
#define OFFSET_STRINGS_BLOCK_SIZE  (8 * 4)

#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE   0x00000002
#define FDT_PROP       0x00000003
#define FDT_NOP        0x00000004
#define FDT_END        0x00000009

#define PARSER_STRING_BUF_SIZE 256

struct parser {
    uint8_t *addr;
    uint8_t current_node_type;

    struct  {
        uint32_t len;
        char buf[PARSER_STRING_BUF_SIZE];
    } string;
};

static void parser_init(struct parser *p, uint8_t *addr) {
    memset8((uint8_t*)p, 0, sizeof(struct parser));
    p->addr = addr;
}

static void parser_read_null_terminated_string(struct parser *p) {
    p->string.len = 0;

    while (*(p->addr) && p->string.len < PARSER_STRING_BUF_SIZE) {
        p->string.buf[p->string.len] = *(p->addr);
        ++p->string.len;
        ++p->addr;
    }

    if (p->string.len == PARSER_STRING_BUF_SIZE) {
        --p->string.len;
    }

    p->string.buf[p->string.len] = '\0';
}

static uint32_t parser_read_u32(struct parser *p) {
    const uint32_t val = mem_read_u32be((uint64_t)p->addr);
    p->addr += 4;
    return bswap_32(val);
}



static void print_reserved_regions(uint64_t reserved_block_addr) {
    uint32_t i = 0;
    const uint32_t LIMIT = 2*5;

    while (i < LIMIT) {
        uint64_t const addr = mem_read_u64be(reserved_block_addr + ((i + 0) * sizeof(uint64_t)));
        uint64_t const size = mem_read_u64be(reserved_block_addr + ((i + 1) * sizeof(uint64_t)));

        if (addr == 0 && size == 0) {
            break;
        }

        printk("[DeviceTree] Reserved Region: Addr=0x%lX, Size=%lu\n", addr, size);

        i += 2;
    }

    printk("[DeviceTree] Finished parsing reserved regions, count = %u\n", i / 2);
}


void devicetree_init(struct devicetree *dt, uint64_t base_address)
{
    memset8((uint8_t *)dt, 0, sizeof(struct devicetree));
    dt->base = base_address;
}

void devicetree_parse(struct devicetree *dt)
{
    struct parser p;
    
    uint32_t const magic = mem_read_u32be(dt->base + OFFSET_MAGIC);
    if (magic != MAGIC) {
        printk("[FATAL] DeviceTree: bad magic\n");
        return;
    }

    uint32_t const version = mem_read_u32be(dt->base + OFFSET_VERSION);
    if (version != 17) {
        printk("[FATAL] DeviceTree: bad version %u\n", version);
        return;
    }

    uint32_t const structs_block_off = mem_read_u32be(dt->base + OFFSET_STRUCTS_BLOCK_OFF);
    uint32_t const structs_block_size = mem_read_u32be(dt->base + OFFSET_STRUCTS_BLOCK_SIZE);
    uint32_t const strings_block_off = mem_read_u32be(dt->base + OFFSET_STRINGS_BLOCK_OFF);
    uint32_t const strings_block_size = mem_read_u32be(dt->base + OFFSET_STRINGS_BLOCK_SIZE);
    uint32_t const reserved_block_off = mem_read_u32be(dt->base + OFFSET_RESERVED_BLOCK_OFF);

    printk("[DeviceTree] Reserved Region Block is at 0x%lX\n", dt->base + reserved_block_off);

    print_reserved_regions(dt->base + reserved_block_off);

    dt->fdt.strings_base = dt->base + strings_block_off;
    dt->fdt.strings_size = strings_block_size;
    dt->fdt.structs_base = dt->base + structs_block_off;
    dt->fdt.structs_size = structs_block_size;

    printk("[DeviceTree] Structs: Base = 0x%lX, Size = %lu\n", dt->fdt.structs_base, dt->fdt.structs_size);
    printk("[DeviceTree] Strings: Base = 0x%lX, Size = %lu\n", dt->fdt.strings_base, dt->fdt.strings_size);




}
