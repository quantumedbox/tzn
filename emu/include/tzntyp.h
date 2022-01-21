#ifndef TZN_TYPH
#define TZN_TYPH

#include <stddef.h> /* TEMP */

#define TZN_NULL ((void*)0)

#define U8      unsigned char
#define U16     unsigned short
#define U32     unsigned int
#define U64     unsigned long long int

#define I8      signed char
#define I16     signed short
#define I32     signed int
#define I64     signed long long int

#define U8_MIN  ((U8)0)
#define U8_MAX  ((U8)-1)

/*
  Unsigned integer representation of pointer types
  TODO size_t isn't guaranteed to be able to hold every pointer value
       we need to define it for each implementation in the future
*/
#define TZN_UPTR  size_t

#define TZN_ERR   U8
#define TZN_BOOL  U8
#define TZN_YES   ((TZN_BOOL)1)
#define TZN_NO    ((TZN_BOOL)0)

#endif
