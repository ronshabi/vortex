// DEVICE TREE EXPLANATION:
// https://android.googlesource.com/kernel/msm/+/android-7.1.0_r0.2/Documentation/devicetree/bindings/arm/gic-v3.txt
// https://developer.arm.com/documentation/198123/0302/Configuring-the-Arm-GIC

// reg (base-addr, size): GICD, GICR, [GICC], [GICH], [GICV]

#pragma once

#define GICD_BASE     0x8000000UL
#define GICD_SIZE     0x10000UL
#define GICR_BASE     0x80a0000UL
#define GICR_SIZE     0xf60000UL
#define GICR_SGI_BASE 0x10000U

#define GICD_REG_CTLR         (GICD_BASE + 0x0UL)
#define GICD_REG_TYPER        (GICD_BASE + 0x4UL)
#define GICD_REG_ISENABLER(n) (GICD_BASE + 0x0100UL + (4 * (n)))

#define GICD_REG_IPRIORITYR(n) (GICD_BASE + 0x0400UL + (4 * (n)))

// GICR

#define GICR_REG_SGI_IPRIORITYR(n)                                             \
    (GICR_BASE + GICR_SGI_BASE + 0x0400UL + (4 * (n)))

#define GICR_REG_SGI_IGROUPR(n)                                                \
    (GICR_BASE + GICR_SGI_BASE + 0x0080UL + (4 * (n)))
#define GICR_REG_SGI_IGRPMODR(n)                                               \
    (GICR_BASE + GICR_SGI_BASE + 0x0D00UL + (4 * (n)))

#define GICR_REG_TYPER          (GICR_BASE + 0x0008UL)
#define GICR_REG_CTLR           (GICR_BASE + 0x0000UL)
#define GICR_REG_WAKER          (GICR_BASE + 0x0014UL)
#define GICR_REG_SGI_ISENABLER0 (GICR_BASE + GICR_SGI_BASE + 0x0100UL)
#define GICR_REG_SGI_IGROUPR0   (GICR_BASE + GICR_SGI_BASE + 0x0080UL)

void init_gic();

void enable_timer();