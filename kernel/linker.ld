ENTRY(_start)
SECTIONS {
    . = 0x40100000;

    .text : {
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .init_array : ALIGN(8) {
        __init_array_start = .;
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;
    }

    .data : {
        *(.data .data.*)
    }

    .bss : {
        . = ALIGN(8);
        __bss_start = .;
        *(.bss .bss.* COMMON)
        . = ALIGN(8);
        __bss_end = .;
    }

    . = ALIGN(8);
    . += 0x1000; /* stack */
    stack_top = .;
}
