#ifndef TZN_COMPILER_H
#define TZN_COMPILER_H

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

#ifndef TZN_ISLE /* Is Little Endian */
  #if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
      defined(__BIG_ENDIAN__) || \
      defined(__ARMEB__) || \
      defined(__THUMBEB__) || \
      defined(__AARCH64EB__) || \
      defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
    #define TZN_ISLE 0
  #elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
      defined(__LITTLE_ENDIAN__) || \
      defined(__ARMEL__) || \
      defined(__THUMBEL__) || \
      defined(__AARCH64EL__) || \
      defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
    #define TZN_ISLE 1
  #else
    #error "can't determine endianness of target, set TZN_ISLE macro to 0 or 1 manually"
  #endif
#endif

#if TZN_ISLE == 1
  #define U16_INIT(low, high) (((U16)high << 8) | (U16)low)
  #define U16_LOW(v) (v >> 8)
  #define U16_HIGH(v) (v & 0x0F)
#else
  #define U16_INIT(low, high) (((U16)low << 8) | (U16)high)
  #define U16_LOW(v) (v & 0x0F)
  #define U16_HIGH(v) (v >> 8)
#endif

/* Subtraction of signed 8 bit integer from 16 unsigned one */
#define U16_S_I8(sbtrhnd, sbtrctr) \
  if (sbtrctr & 0x80) \
    sbtrhnd = (sbtrhnd - (U8)~sbtrctr) - 1; \
  else \
    sbtrhnd += sbtrctr;

#endif
