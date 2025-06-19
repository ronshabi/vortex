.text

.macro vt_entry_not_implemented
    .align 7
    str x30, [sp, #-0xF0] // Hack: C's exception_info->regs[30]
    bl backup_regs
    mov x0, sp
    b panic
.endm


backup_regs:
    sub sp, sp, #-288 // The size of struct exception_info

    stp  x0,  x1, [sp, #0x00]
    stp  x2,  x3, [sp, #0x10]
    stp  x4,  x5, [sp, #0x20]
    stp  x6,  x7, [sp, #0x30]
    stp  x8,  x9, [sp, #0x40]
    stp x10, x11, [sp, #0x50]
    stp x12, x13, [sp, #0x60]
    stp x14, x15, [sp, #0x70]
    stp x16, x17, [sp, #0x80]
    stp x18, x19, [sp, #0x90]
    stp x20, x21, [sp, #0xA0]
    stp x22, x23, [sp, #0xB0]
    stp x24, x25, [sp, #0xC0]
    stp x26, x27, [sp, #0xD0]
    stp x28, x29, [sp, #0xE0]

    mrs x0, CurrentEL
    str x0, [sp, #0xF8]

    mrs x0, ELR_EL1
    str x0, [sp, #0x100]

    mrs x0, ESR_EL1
    str x0, [sp, #0x108]

    mrs x0, SPSR_EL1
    str x0, [sp, #0x110]
    
    mrs x0, FAR_EL1
    str x0, [sp, #0x118]

    ret

//
// AArch64 manual: exception vector tables should be 2048-aligned
//
.align 11
.type vector_table_el1, @object
.global vector_table_el1
vector_table_el1:
    // +++ From the current EL, with SP_EL0 +++

    // Offset: 0x000, Type: Synchronous
    vt_entry_not_implemented

    // Offset: 0x080, Type: IRQ
    vt_entry_not_implemented

    // Offset: 0x100, Type: FIQ
    vt_entry_not_implemented

    // Offset: 0x180, Type: SError
    vt_entry_not_implemented


    // +++ From the current EL, with SP_ELx +++


    // Offset: 0x200, Type: Synchronous
    vt_entry_not_implemented

    // Offset: 0x280, Type: IRQ    
    // .align 7
    // b prepare_irq
    vt_entry_not_implemented

    // Offset: 0x300, Type: FIQ
    vt_entry_not_implemented

    // Offset: 0x380, Type: SError
    vt_entry_not_implemented

    // +++ From a lower EL, lower is AArch64 (unused) +++
    vt_entry_not_implemented        // Offset: 0x400, Synchronous
    vt_entry_not_implemented        // Offset: 0x480, IRQ
    vt_entry_not_implemented        // Offset: 0x500, FIQ
    vt_entry_not_implemented        // Offset: 0x580, SError
    // +++ From a lower EL, lower is AArch32 (unused) +++
    vt_entry_not_implemented        // Offset: 0x600, Synchronous
    vt_entry_not_implemented        // Offset: 0x680, IRQ
    vt_entry_not_implemented        // Offset: 0x700, FIQ
    vt_entry_not_implemented        // Offset: 0x780, SError

.size vector_table_el1, (. - vector_table_el1)
