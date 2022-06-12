#if !defined(TCMPLR_H)
#define TCMPLR_H

#include "tzntyp.h"

#if defined(__CC65__)
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

#if defined(__GNUC__)
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

#if defined(__GNUC__)
  #if !defined(NDEBUG)
    #define TZN_DEAD() T_ASSERT(0, "Reached unreachable")
  #else
    #define TZN_DEAD() __builtin_unreachable()
  #endif
#else
  #if !defined(NDEBUG)
    #define TZN_DEAD() T_ASSERT(0, "Reached unreachable")
  #else
    #define TZN_DEAD() /* TODO Shouldn't be just ignored */
  #endif
#endif

#endif
