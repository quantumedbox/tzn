#ifndef TCMPLR_H
#define TCMPLR_H

#include "tzntyp.h"
#include "tznlmt.h"

#ifdef __CC65__
  #define T_HOTMEM(decl, sym) \
    _Pragma("bss-name (push, \"ZEROPAGE\")") \
    _Pragma("data-name (push, \"ZEROPAGE\")") \
    decl; \
    _Pragma("bss-name (pop)") \
    _Pragma("data-name (pop)") \
    _Pragma("zpsym ("#sym")")
#else
  #define T_HOTMEM(decl, _) decl
#endif

#ifdef __GNUC__
  #define T_NORET   __attribute__((noreturn))
  #define T_HOT     __attribute__((hot))
  #define T_COLD    __attribute__((cold))
  #define T_INLINE  __attribute__((always_inline))
#else /* Unknown compiler */
  #define T_NORET   /* No effect */
  #define T_HOT     /* No effect */
  #define T_COLD    /* No effect */
  #define T_INLINE  inline
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

#endif
