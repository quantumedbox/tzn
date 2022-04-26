#ifndef T_ERRH
#define T_ERRH

#include "tcmplr.h"

#ifndef TZN_RLS
  #ifndef __CC65__
    #define TZN_ASRT(cond, literal) (cond) ? (void)0 : tError(__FILE__ ":" TZN_STR(__LINE__) ": \"" literal "\"")
  #else
    /* It appears that CC65 doesn't know how to handle string literal concatenation in this case, so, file and line information is ignored */
    #define TZN_ASRT(cond, literal) (cond) ? (void)0 : tError(literal)
  #endif
#else
  #define TZN_ASRT(cond, literal) (void)(0)
#endif

#endif
