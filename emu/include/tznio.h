/*
  System communication abstraction

  TODO Abstract all of libc in here
*/

#ifndef TZN_IO_H
#define TZN_IO_H

/*
  @brief  Print null terminated string holding error message and then exit
  @warn   Preferably only const literals should be passed here, don't mess with null strings
*/
TZN_NORETURN void tznError(const char* literal);

#endif
