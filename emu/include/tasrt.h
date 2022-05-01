#ifndef T_ERRH
#define T_ERRH

#include "tutils.h"

#ifndef TZN_RLS
  #ifndef __CC65__
    #define T_ASSERT(cond, literal) if (cond) {} else tError(__FILE__ ":" T_STREXP(__LINE__) ": \"" literal "\"")
  #else
    /* It appears that CC65 doesn't know how to handle string literal concatenation in this case, so, file and line information is ignored */
    #define T_ASSERT(cond, literal) if (cond) {} else tError(literal)
  #endif
#else
  #define T_ASSERT(cond, literal) (void)(0)
#endif

#endif
