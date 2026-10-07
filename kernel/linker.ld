ENTRY(_start)
SECTIONS {
    . = 0x40100000;

    .text : {
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    .bss : {
        __bss_start = .;
        *(.bss .bss.* COMMON)
        __bss_end = .;
    }

    . = ALIGN(8);
    . += 0x1000; /* stack */
    stack_top = .;
}
