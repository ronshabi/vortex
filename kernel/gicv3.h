// Good docs
// https://developer.arm.com/documentation/198123/0302/Configuring-the-Arm-GIC
// https://developer.arm.com/documentation/ihi0048/a/Glossary
// https://developer.arm.com/documentation/ddi0595/2021-03/AArch32-Registers/CNTV-TVAL--Counter-timer-Virtual-Timer-TimerValue-register

// IN DTS: reg (base-addr, size) := <GICD, GICR, [GICC], [GICH], [GICV]>
// https://android.googlesource.com/kernel/msm/+/android-7.1.0_r0.2/Documentation/devicetree/bindings/arm/gic-v3.txt

#pragma once

#define GICD_BASE     0x8000000UL
#define GICD_SIZE     0x10000UL
#define GICR_BASE     0x80a0000UL
#define GICR_SIZE     0xf60000UL
#define GICR_SGI_BASE 0x10000U


#define GICD_REG_CTLR         (GICD_BASE + 0x0UL)
#define GICR_REG_WAKER          (GICR_BASE + 0x0014UL)
#define GICR_REG_SGI_ISENABLER0 (GICR_BASE + GICR_SGI_BASE + 0x0100UL)
#define GICR_REG_SGI_IGROUPR0   (GICR_BASE + GICR_SGI_BASE + 0x0080UL)

void init_gic();