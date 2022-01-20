#ifndef TZN_LMTH
#define TZN_LMTH

/*
  TODO It seems to be possible to not have ability to get full range of bytes when it's dependent on full range of possibe values
*/

#define TZN_REGC 9 /* Register Count */
#define TZN_REGS 3 /* Reserved Registers */
#define TZN_REGG (TZN_REGC - TZN_REGS) /* General Purpose Registers */

#define TZN_MTOB 40960 /* Total Bytes, 40 KB */

#define TZN_SPIN TZN_MTOB /* Stack Pointer Initial */

#endif
