#ifndef TZN_LMTH
#define TZN_LMTH

/*
  TODO It seems to be possible to not have ability to get full range of bytes when it's dependent on full range of possibe values
*/

#define TZN_REGC 9 /* Register Count */
#define TZN_REGS 3 /* Reserved Registers */
#define TZN_REGG (TZN_REGC - TZN_REGS) /* General Purpose Registers */

#ifdef __C64__
  #define TZN_MTOB (U16)35840U /* Total Bytes, 35 KB */
#else
  #define TZN_MTOB (U16)40960U /* Total Bytes, 40 KB */
#endif

#define TZN_SPIN TZN_MTOB /* Stack Pointer Initial */

#endif
