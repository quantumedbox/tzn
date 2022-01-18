#ifndef TZN_COMPILER_H
#define TZN_COMPILER_H

#include "tzntyp.h"
#include "tznlmt.h"

#define TZN_VOID ((void*)0)

#ifdef __GNUC__
  #define TZN_NORETURN  __attribute__((noreturn))
  #define TZN_LIKELY    __attribute__((hot))
  #define TZN_UNLIKELY  __attribute__((cold))
  #define TZN_INLINE    __attribute__((always_inline))
  #define TZN_NOINLINE  __attribute__((noinline))
#else /* Unknown compiler */
  #define TZN_NORETURN  /* No effect */
  #define TZN_LIKELY    /* No effect */
  #define TZN_UNLIKELY  /* No effect */
  #define TZN_INLINE    inline
  #define TZN_NOINLINE  /* No effect */
#endif

#include "tznio.h"

#ifdef TZN_DEBUG
  #define TZN_ASSERT(cond, literal) do { if (!(cond)) tzn_Error(literal); } while (0)
#else
  #define TZN_ASSERT(cond, literal) /* No effect */
#endif

#ifdef __GNUC__
  #ifdef TZN_DEBUG
    #define TZN_UNREACHABLE() TZN_ASSERT(0, "reached unreachable")
  #else
    #define TZN_UNREACHABLE() __builtin_unreachable()
  #endif
#else
  #ifdef TZN_DEBUG
    #define TZN_UNREACHABLE() TZN_ASSERT(0, "reached unreachable")
  #else
    #define TZN_UNREACHABLE() /* No effect */ /* TODO Shouldn't be just ignored */
  #endif
#endif

#ifndef TZN_IS_LITTLE_ENDIAN
  #if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
      defined(__BIG_ENDIAN__) || \
      defined(__ARMEB__) || \
      defined(__THUMBEB__) || \
      defined(__AARCH64EB__) || \
      defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
    #define TZN_IS_LITTLE_ENDIAN 0
  #elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
      defined(__LITTLE_ENDIAN__) || \
      defined(__ARMEL__) || \
      defined(__THUMBEL__) || \
      defined(__AARCH64EL__) || \
      defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
    #define TZN_IS_LITTLE_ENDIAN 1
  #else
    #error "Can't determine endianness of target, set TZN_IS_LITTLE_ENDIAN macro to 0 or 1 manually"
  #endif
#endif

#if TZN_IS_LITTLE_ENDIAN == 1
  #define U16_COMPOSE(low, high) (((U16)high << 8) | (U16)low)
  #define U16_LOW(v) (v >> 8)
  #define U16_HIGH(v) (v & 0x0F)
#else
  #define U16_COMPOSE(low, high) (((U16)low << 8) | (U16)high)
  #define U16_LOW(v) (v & 0x0F)
  #define U16_HIGH(v) (v >> 8)
#endif

#define U16_SUB_I8(subtrahend, subtractor) \
  if (rel_addr & 0x80) \
    program_counter = (program_counter - (U8)~rel_addr) - 1; \
  else \
    program_counter += rel_addr;

#endif
