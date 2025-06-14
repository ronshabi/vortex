// Good docs
// https://developer.arm.com/documentation/198123/0302/Configuring-the-Arm-GIC
// https://developer.arm.com/documentation/ihi0048/a/Glossary
// https://developer.arm.com/documentation/ddi0595/2021-03/AArch32-Registers/CNTV-TVAL--Counter-timer-Virtual-Timer-TimerValue-register

// IN DTS: reg (base-addr, size) := <GICD, GICR, [GICC], [GICH], [GICV]>
// https://android.googlesource.com/kernel/msm/+/android-7.1.0_r0.2/Documentation/devicetree/bindings/arm/gic-v3.txt

#include <stdint.h>

#pragma once

void gicv3_init();
void gicv3_enable_interrupt(uint64_t intno);