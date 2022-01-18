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
  #define TZN_ASRT(cond, literal) do { \
    if (!(cond)) tznError(__FILE__ ":" TZN_STR(__LINE__) ": \"" literal "\""); \
  } while (0)
#else
  #define TZN_ASRT(cond, literal) ((void)0)
#endif

#endif
