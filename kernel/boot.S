.global _start

.text
.align 11 // 2048

_start: 
    // jump to C
    ldr x30, =stack_top
    mov sp, x30
    bl kmain
    b halt


.global halt
.type halt, @function
halt:
    b .
    
    .size halt, (. - halt)
