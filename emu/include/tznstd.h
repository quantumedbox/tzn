#ifndef TZN_COMH
#define TZN_COMH

#include "tzntyp.h"
#include "tznlmt.h"

#ifdef __GNUC__
  #define TZN_NORE  __attribute__((noreturn))
  #define TZN_HOT   __attribute__((hot))
  #define TZN_COLD  __attribute__((cold))
  #define TZN_INL   __attribute__((always_inline))
  #define TZN_NINL  __attribute__((noinline))
#else /* Unknown compiler */
  #define TZN_NORE  /* No effect */
  #define TZN_HOT   /* No effect */
  #define TZN_COLD  /* No effect */
  #define TZN_INL   inline
  #define TZN_NINL  /* No effect */
#endif

#ifdef __GNUC__
  #ifndef TZN_RELEASE
    #define TZN_DEAD() TZN_ASRT(0, "reached unreachable")
  #else
    #define TZN_DEAD() __builtin_unreachable()
  #endif
#else
  #ifndef TZN_RELEASE
    #define TZN_DEAD() TZN_ASRT(0, "reached unreachable")
  #else
    #define TZN_DEAD() ((void)0) /* TODO Shouldn't be just ignored */
  #endif
#endif

/* TODO
  Side-effect macros might cause problems
  Better swap them to pure functions
*/

#define U16_INIT(low, high) (((U16)(high) << 8) | (U16)(low))
#define U16_LOW(v) ((v) & 0x00FF)
#define U16_HIGH(v) (((v) & 0xFF00) >> 8)
/* Set low byte */
#define U16_LSET(v, byte) do { v = (v & 0xFF00) | (byte); } while (0)
/* Set high byte */
#define U16_HSET(v, byte) do { v = (v & 0x00FF) | ((byte) << 8); } while (0)

/* Subtraction of signed 8 bit integer from 16 unsigned one */
#define U16_S_I8(sbtrhnd, sbtrctr) \
  if ((sbtrctr) & 0x80) \
    (sbtrhnd) = ((sbtrhnd) - (U8)~(sbtrctr)) - 1; \
  else \
    (sbtrhnd) += (sbtrctr);

#endif
