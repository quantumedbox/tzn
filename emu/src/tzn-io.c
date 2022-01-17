#include <stdio.h>
#include <stdlib.h>

#include "tzn-std.h"

/* TODO Abstract IO operations for portability on various libc implementations (Plan 9 for example) */

TZN_NORETURN
void
tzn_Error(const char* literal)
{
  fputs("[error: ", stderr);
  fputs(literal, stderr);
  fputc(']', stderr);
  abort();
}