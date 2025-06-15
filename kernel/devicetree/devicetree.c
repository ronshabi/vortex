#include "devicetree.h"
#include "memory.h"
#include "printk.h"

#include <klibc/endian.h>
#include <klibc/string.h>

#define MAGIC 0xD00DFEEDUL

#define OFFSET_MAGIC   0
#define OFFSET_VERSION (5 * 4)

#define OFFSET_RESERVED_BLOCK_OFF (4 * 4)
#define OFFSET_STRUCTS_BLOCK_OFF  (2 * 4)
#define OFFSET_STRUCTS_BLOCK_SIZE (9 * 4)
#define OFFSET_STRINGS_BLOCK_OFF  (3 * 4)
#define OFFSET_STRINGS_BLOCK_SIZE (8 * 4)

#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE   0x00000002
#define FDT_PROP       0x00000003
#define FDT_NOP        0x00000004
#define FDT_END        0x00000009

#define PARSER_STRING_BUF_SIZE 256

#define PRINT_ERROR(p, fmt, ...)                                               \
    do                                                                         \
    {                                                                          \
        printk("[DeviceTree] Parser Error: " fmt "\n" __VA_OPT__(, ));         \
        printk("\tAddr: 0x%p\n", p->addr);                                     \
    } while (0)

struct parser_string
{
    uint32_t len;
    char     buf[PARSER_STRING_BUF_SIZE];
};

struct parser
{
    uint8_t *addr;
    uint8_t *end;
    uint8_t *stringblock;
    uint32_t  current_node_type;
    uint32_t current_depth;
    uint32_t is_at_end_node;

    struct devicetree *dt;

    uint32_t prop_len;
    char *prop_name;

    struct parser_string node_name;
    struct parser_string prop_value;
};

static void parser_init(struct parser *p, uint8_t *struct_block_addr,
                        uint64_t size, struct devicetree *dt)
{
    memset8((uint8_t *)p, 0, sizeof(struct parser));
    p->addr = struct_block_addr;
    p->end  = p->addr + size;
    p->stringblock = (uint8_t*)(dt->fdt.strings_base);
    p->dt   = dt;
}

// Advance until we reach an aligned address
// For example, if alignment=4, then we advance until we hit an address
// with last 2 bits as 00.
static void parser_advance_to_aligned_addr(struct parser *p, uint64_t alignment)
{
    while (((uint64_t)p->addr) & (alignment - 1))
    {
        ++p->addr;
    }
}

static uint32_t parser_read_u32(struct parser *p)
{
    const uint32_t val = mem_read_u32be((uint64_t)p->addr);
    p->addr += sizeof(uint32_t);
    return val;
}

static void parser_read_null_terminated_string(struct parser        *p,
                                               struct parser_string *to)
{
    to->len = 0;

    while (*(p->addr) && to->len < PARSER_STRING_BUF_SIZE)
    {
        to->buf[to->len] = *(p->addr);
        ++to->len;
        ++p->addr;
    }

    if (to->len == PARSER_STRING_BUF_SIZE)
    {
        --to->len;
    }

    to->buf[to->len] = '\0';

    // Skip the null term
    ++p->addr;

    parser_advance_to_aligned_addr(p, 4);
}

static int parse(struct parser *p)
{
    p->current_node_type = 0;

    while (p->addr < p->end)
    {
        // Get Node Type
        p->current_node_type = parser_read_u32(p);
        p->is_at_end_node = 0;

        if (p->current_node_type == FDT_BEGIN_NODE) {
            parser_read_null_terminated_string(p, &p->node_name);
            
            printk("~> begin (depth %u): '%s'\n", p->current_depth, p->node_name.buf);
            
            ++p->current_depth;
            continue;
        }

        if (p->current_node_type == FDT_PROP) {
            p->prop_len = parser_read_u32(p);
            p->prop_name = (char*)p->stringblock + parser_read_u32(p);

            // if (p->prop_name == (char*)p->stringblock)

            printk("~> got prop '%s' of length %u\n", p->prop_name , p->prop_len);

            if (p->prop_len) {
                parser_read_null_terminated_string(p, &p->prop_value);
                printk("\t~> prop value '%s'\n", p->prop_value.buf);
            }
            
            // for (uint32_t i = 0; i < p->prop_len; ++i) {
            //     ++p->addr;
            // }



            parser_advance_to_aligned_addr(p, 4);
            continue;
        }

        if (p->current_node_type == FDT_NOP) {
            printk("~> got nop\n");
            continue;
        }

        if (p->current_node_type == FDT_END_NODE) {
            if (p->current_depth == 0) {
                PRINT_ERROR(p, "[!] Attempt to close node when depth is 0");
                return 1;
            }

            --p->current_depth;
            printk("-- end --\n");
            p->is_at_end_node = 1;
            continue;
        }

        if (p->current_node_type == FDT_END) {
            if (!p->is_at_end_node) {
                PRINT_ERROR(p, "FDT_END came before FDT_END_NODE");
                return 1;
            }

            printk("~> end parsing\n");
            p->is_at_end_node = 0;
            return 0;
        }
    }

    return 0;
}

static void print_reserved_regions(uint64_t reserved_block_addr)
{
    uint32_t       i     = 0;
    const uint32_t LIMIT = 2 * 5;

    while (i < LIMIT)
    {
        uint64_t const addr =
            mem_read_u64be(reserved_block_addr + ((i + 0) * sizeof(uint64_t)));
        uint64_t const size =
            mem_read_u64be(reserved_block_addr + ((i + 1) * sizeof(uint64_t)));

        if (addr == 0 && size == 0)
        {
            break;
        }

        printk("[DeviceTree] Reserved Region: Addr=0x%lX, Size=%lu\n", addr,
               size);

        i += 2;
    }

    printk("[DeviceTree] Finished parsing reserved regions, count = %u\n",
           i / 2);
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
    if (magic != MAGIC)
    {
        printk("[FATAL] DeviceTree: bad magic\n");
        return;
    }

    uint32_t const version = mem_read_u32be(dt->base + OFFSET_VERSION);
    if (version != 17)
    {
        printk("[FATAL] DeviceTree: bad version %u\n", version);
        return;
    }

    uint32_t const structs_block_off =
        mem_read_u32be(dt->base + OFFSET_STRUCTS_BLOCK_OFF);
    uint32_t const structs_block_size =
        mem_read_u32be(dt->base + OFFSET_STRUCTS_BLOCK_SIZE);
    uint32_t const strings_block_off =
        mem_read_u32be(dt->base + OFFSET_STRINGS_BLOCK_OFF);
    uint32_t const strings_block_size =
        mem_read_u32be(dt->base + OFFSET_STRINGS_BLOCK_SIZE);
    uint32_t const reserved_block_off =
        mem_read_u32be(dt->base + OFFSET_RESERVED_BLOCK_OFF);

    printk("[DeviceTree] Reserved Region Block is at 0x%lX\n",
           dt->base + reserved_block_off);

    print_reserved_regions(dt->base + reserved_block_off);

    dt->fdt.strings_base = dt->base + strings_block_off;
    dt->fdt.strings_size = strings_block_size;
    dt->fdt.structs_base = dt->base + structs_block_off;
    dt->fdt.structs_size = structs_block_size;

    printk("[DeviceTree] Structs: Base = 0x%lX, Size = %lu\n",
           dt->fdt.structs_base, dt->fdt.structs_size);
    printk("[DeviceTree] Strings: Base = 0x%lX, Size = %lu\n",
           dt->fdt.strings_base, dt->fdt.strings_size);

    parser_init(&p, (uint8_t *)(dt->fdt.structs_base), dt->fdt.structs_size, dt);

    if (parse(&p))
    {
        printk("[DeviceTree] Errors in parsing the dtb!\n");
    }
}
