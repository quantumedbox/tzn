#ifndef TZN_LIMITS_H
#define TZN_LIMITS_H

#define TZN_REGC 9 /* Register Count */
#define TZN_REGS 3 /* Reserved Registers */
#define TZN_REGG (TZN_REGC - TZN_REGS) /* General Purpose Registers */

#define TZN_MTOB 65535 /* Totall Bytes */

#define TZN_MRES 0x0000 /* Reserved Start */
#define TZN_MREE 0x00FF /* Reserved End */
#define TZN_MREB (TZN_MREE - TZN_MRES) /* Reserved Bytes */

#define TZN_MDVS TZN_MRES /* Devices Start */
#define TZN_MDVE 0x001F /* Devices End */

#define TZN_MRMS 0x0100 /* RAM Start */
#define TZN_MRME 0xFFFF /* RAM End */
#define TZN_MRMB (TZN_MRME - TZN_MRMS) /* RAM Bytes */

#define TZN_SPIN TZN_MRME /* Stack Pointer Initial */

#endif
