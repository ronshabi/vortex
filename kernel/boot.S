.global _start

.text
.align 11 // 2048

_start:
    //
    // Set up the EL1 vector table
    //
    ldr x0, =vector_table_el1
    msr VBAR_EL1, x0
    isb

    //
    // Go to C's kmain()
    //
    ldr x30, =stack_top

    mov sp, x30
    bl kmain

    //
    // If we got here, we should halt
    //
    b halt

