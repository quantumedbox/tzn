/*
  IO interface

  TODO Abstract all of libc in here
  TODO Usage of libc implementation of CC65 makes for duplication of some code
       For example, puts essentially doubles the same code that is already present in trm device
*/

#ifndef TSYS_H
#define TSYS_H

#include <string.h>

#include "tcmplr.h"

/* File operation modes */
#define TZN_FL0  0x00 /* Default */
#define TZN_FLIS 0x01 /* Ignore size */
#define TZN_FLRW 0x02 /* Rewrite */

/* TODO Do they make sense be here? */
#define tMemSet(dest, val, bytes)   memset(dest, val, (size_t)bytes)
#define tMemCopy(dest, src, bytes)  memcpy(dest, src, (size_t)bytes)

void tTrmPrLt(const char* literal); /* Use terminal device implementation for printing */

/*
  @brief  Print null terminated string holding error message and then exit
  @warn   Preferably only const literals should be passed here, don't mess with null strings
*/
#define tError(literal) do { \
  tTrmPrLt("error: "); \
  tTrmPrLt(literal); \
  exit(EXIT_FAILURE); \
} while (0)

#define tPrint(literal) tTrmPrLt(literal)

#ifndef TZN_RLS
  #define tznLog(literal) tPrint(literal)
#else
  #define tznLog(literal) (void)(0)
#endif

#ifdef T_FLSYS
  /*
    @brief  Read contents of file at filename to memory buffer of specified size
            Flags dictate behaviour
    @params was_read -- Count of bytes read, optional
    @return Status code, 0 is success
  */
  T_ERR tznFlRd(const char* filename, T_U8* memory, T_U16 size, T_U16* was_read, T_U8 flags);

  /*
    @brief  Write buffer to a file
  */
  T_ERR tznFlWr(const char* filename, T_U8* memory, T_U16 size, T_U8 flags);
#endif

#endif
