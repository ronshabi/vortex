.global _start

.text
.align 11 // 2048

_start:
    // Set up the EL1 vector table
    ldr x0, =vector_table_el1
    msr VBAR_EL1, x0
    isb

    // Enable IRQs
    msr DAIFCLR, #0x2

    // Go to C's kmain()
    ldr x30, =stack_top

    mov sp, x30

    // Zero bss
    ldr x0, =__bss_start
    ldr x1, =__bss_end
1:  cmp x0, x1          // are we done with the clean loop?
    b.hs 2f             // yes, we are done
    str xzr, [x0], #8   // x0+=8
    b 1b                // next loop iteration

2: 
    bl cxx_run_global_ctors
    bl kmain

    // If we got here, we should halt
    b halt

