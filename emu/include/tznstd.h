#ifndef TZN_COMH
#define TZN_COMH

#include "tzntyp.h"
#include "tznlmt.h"

#ifdef __GNUC__
  #define T_NORET  __attribute__((noreturn))
  #define TZN_HOT   __attribute__((hot))
  #define TZN_COLD  __attribute__((cold))
  #define TZN_INL   __attribute__((always_inline))
  #define TZN_NINL  __attribute__((noinline))
#else /* Unknown compiler */
  #define T_NORET  /* No effect */
  #define TZN_HOT   /* No effect */
  #define TZN_COLD  /* No effect */
  #define TZN_INL   inline
  #define TZN_NINL  /* No effect */
#endif

#ifdef __GNUC__
  #ifndef TZN_RLS
    #define TZN_DEAD() TZN_ASRT(0, "Reached unreachable")
  #else
    #define TZN_DEAD() __builtin_unreachable()
  #endif
#else
  #ifndef TZN_RLS
    #define TZN_DEAD() TZN_ASRT(0, "Reached unreachable")
  #else
    #define TZN_DEAD() ((void)0) /* TODO Shouldn't be just ignored */
  #endif
#endif

/* TODO
  Side-effect macros might cause problems
  Better swap them to pure functions
*/

#define T_U16_INIT(low, high) (((T_U16)(high) << 8) | (T_U16)(low))
#define T_U16_LOW(v) ((v) & 0x00FF)
#define T_U16_HIGH(v) (((v) & 0xFF00) >> 8)
/* Set low byte */
#define T_U16_LSET(v, byte) do { v = (v & 0xFF00) | (byte); } while (0)
/* Set high byte */
#define T_U16_HSET(v, byte) do { v = (v & 0x00FF) | ((byte) << 8); } while (0)

/* Subtraction of signed 8 bit integer from 16 unsigned one */
#define T_U16SI8(sbtrhnd, sbtrctr) \
  if ((sbtrctr) & 0x80) \
    (sbtrhnd) = ((sbtrhnd) - (T_U8)~(sbtrctr)) - 1; \
  else \
    (sbtrhnd) += (sbtrctr);

#define TZN_STR_(x) #x
#define TZN_STR(x) TZN_STR_(x)

#endif
