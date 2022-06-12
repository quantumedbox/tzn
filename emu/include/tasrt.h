#if !defined(TASRT_T)
#define TASRT_T

#include "tutils.h"

#if !defined(NDEBUG)
  #if !defined(__CC65__)
    #define T_ASSERT(cond, literal) if (cond) {} else tError(__FILE__ ":" T_STREXP(__LINE__) ": \"" literal "\"")
  #else
    /* It appears that CC65 doesn't know how to handle string literal concatenation in this case, so, file and line information is ignored */
    #define T_ASSERT(cond, literal) if (cond) {} else tError(literal)
  #endif
#else
  #define T_ASSERT(cond, literal) (void)(0)
#endif

#endif
