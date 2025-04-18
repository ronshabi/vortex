// DEVICE TREE EXPLANATION:
// https://android.googlesource.com/kernel/msm/+/android-7.1.0_r0.2/Documentation/devicetree/bindings/arm/gic-v3.txt

/*

reg (base-addr, size): GICD, GICR, [GICC], [GICH], [GICV]

*/

#pragma once

#define GICD_BASE 0x8000000
#define GICD_SIZE 0x10000
#define GICR_BASE 0x80a0000
#define GICR_SIZE 0xf60000

