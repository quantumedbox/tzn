#ifndef TZN_ERRH
#define TZN_ERRH

#include "tznstd.h"

/*
  @brief  Print null terminated string holding error message and then exit
  @warn   Preferably only const literals should be passed here, don't mess with null strings
*/
TZN_NORE void tznError(const char* literal);

#define TZN_STRL(x) #x
#define TZN_STR(x) TZN_STRL(x)

#ifndef TZN_RELEASE
  #ifndef __CC65__
    #define TZN_ASRT(cond, literal) (cond) ? (void)0 : tznError(__FILE__ ":" TZN_STR(__LINE__) ": \"" literal "\"")
  #else
    /* It appears that CC65 doesn't know how to handle string literal concatenation in this case, so, file and line information is ignored */
    #define TZN_ASRT(cond, literal) (cond) ? (void)0 : tznError(literal)
  #endif
#else
  #define TZN_ASRT(cond, literal) ((void)0)
#endif

#endif
