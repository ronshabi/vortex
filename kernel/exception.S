.global vector_table_el1
.text


// I don't know how to transfer sizeof objects from C so we must
// cross our fingers this works
.equ size_of_exception_info_struct, 288

//
// Why align 7?
// 2^7 = 128 bytes each section, according to the AArch64
// exception manual.
//
.macro vt_entry_not_implemented
    .align 7
    b panic_prepare
.endm

.macro BACKUP_REGISTERS
// let's prepare all info to pass to struct Exception_Context
    sub sp, sp, #size_of_exception_info_struct

    // load 'em all, 2 registers are 16 bytes = 0x10
    // uint64_t regs[AARCH64_GENERAL_REGISTERS]
    ldp x0, x1,  [sp, #0x00]
    ldp x2, x3,  [sp, #0x10]
    ldp x4, x5,  [sp, #0x20]
    ldp x6, x7,  [sp, #0x30]
    ldp x8, x9,  [sp, #0x40]
    ldp x10, x11, [sp, #0x50]
    ldp x12, x13, [sp, #0x60]
    ldp x14, x15, [sp, #0x70]
    ldp x16, x17, [sp, #0x80]
    ldp x18, x19, [sp, #0x90]
    ldp x20, x21, [sp, #0xA0]
    ldp x22, x23, [sp, #0xB0]
    ldp x24, x25, [sp, #0xC0]
    ldp x26, x27, [sp, #0xD0]
    ldp x28, x29, [sp, #0xE0]
    str x30,      [sp, #0xF0]

    // uint64_t current_el
    mrs x0, CurrentEL
    str x0, [sp, #0xF8]

    // uint64_t elr_elx
    mrs x0, ELR_EL1
    str x0, [sp, #0x100]

    // uint64_t esr_elx
    mrs x0, ESR_EL1
    str x0, [sp, #0x108]

    // uint64_t spsr_elx
    mrs x0, SPSR_EL1
    str x0, [sp, #0x110]

    // uint64_t far_elx (if there is one, else its just garbage)
    mrs x0, FAR_EL1
    str x0, [sp, #0x118]

    // now load sp to the first arg (x0) so we can use it in C
    mov x0, sp
.endm


.macro RESTORE_REGISTERS
    add sp, sp, #size_of_exception_info_struct

    // load 'em all, 2 registers are 16 bytes = 0x10
    // uint64_t regs[AARCH64_GENERAL_REGISTERS]
    ldp x0, x1,  [sp, #0x00]
    ldp x2, x3,  [sp, #0x10]
    ldp x4, x5,  [sp, #0x20]
    ldp x6, x7,  [sp, #0x30]
    ldp x8, x9,  [sp, #0x40]
    ldp x10, x11, [sp, #0x50]
    ldp x12, x13, [sp, #0x60]
    ldp x14, x15, [sp, #0x70]
    ldp x16, x17, [sp, #0x80]
    ldp x18, x19, [sp, #0x90]
    ldp x20, x21, [sp, #0xA0]
    ldp x22, x23, [sp, #0xB0]
    ldp x24, x25, [sp, #0xC0]
    ldp x26, x27, [sp, #0xD0]
    ldp x28, x29, [sp, #0xE0]
    str x30,      [sp, #0xF0]

.endm


//
// AArch64 manual: exception vector tables should be 2048-aligned
//
.align 11
.type vector_table_el1, @object
vector_table_el1:
    //
    // From the current EL, with SP_EL0
    //
    
    // Offset :     0x000
    // Type   :     Synchronous
    vt_entry_not_implemented
    
    // Offset :     0x080
    // Type   :     IRQ
    vt_entry_not_implemented

    // Offset :     0x100
    // Type   :     FIQ
    vt_entry_not_implemented

    // Offset :     0x180
    // Type   :     SError
    vt_entry_not_implemented

    // ----------------------------------------------------

    //
    // From the current EL, with SP_ELx
    //

    // Offset :     0x200
    // Type   :     Synchronous
    vt_entry_not_implemented
    
    // Offset :     0x280
    // Type   :     IRQ
    .align 7
    b prepare_irq

    // Offset :     0x300
    // Type   :     FIQ
    vt_entry_not_implemented

    // Offset :     0x380
    // Type   :     SError
    vt_entry_not_implemented


    // ----------------------------------------------------

    //
    // From a lower EL, lower is AArch64
    //

    // Offset :     0x400
    // Type   :     Synchronous
    vt_entry_not_implemented
    
    // Offset :     0x480
    // Type   :     IRQ
    vt_entry_not_implemented

    // Offset :     0x500
    // Type   :     FIQ
    vt_entry_not_implemented

    // Offset :     0x580
    // Type   :     SError
    vt_entry_not_implemented


    // ----------------------------------------------------

    //
    // From a lower EL, lower is AArch32
    //

    // Offset :     0x600
    // Type   :     Synchronous
    vt_entry_not_implemented
    
    // Offset :     0x680
    // Type   :     IRQ
    vt_entry_not_implemented

    // Offset :     0x700
    // Type   :     FIQ
    vt_entry_not_implemented

    // Offset :     0x780
    // Type   :     SError
    vt_entry_not_implemented

.size vector_table_el1, (. - vector_table_el1)


.global panic_prepare
.type panic_prepare, @function
panic_prepare:
    BACKUP_REGISTERS
    
    // goodbye
    b panic
    
.size panic_prepare, (. - panic_prepare)


.global prepare_irq
.type prepare_irq, @function
prepare_irq:
    BACKUP_REGISTERS
    bl irq_handler
    RESTORE_REGISTERS
    eret

.size prepare_irq, (. - prepare_irq)
