#ifndef TZN_TYPH
#define TZN_TYPH

#include <stddef.h> /* TEMP */

#define T_NULL    ((void*)0)

#define T_U8      unsigned char
#define T_U16     unsigned short

#define T_I8      signed char
#define T_I16     signed short

#define T_U8_MIN  ((T_U8)0)
#define T_U8_MAX  ((T_U8)-1)

/*
  Unsigned integer representation of pointer types
  TODO size_t isn't guaranteed to be able to hold every pointer value
       we need to define it for each implementation in the future
*/
#define T_UPTR    size_t

#define T_ERR     T_U8
#define T_BOOL    T_U8
#define T_TRUE    ((T_BOOL)1)
#define T_FALSE   ((T_BOOL)0)

#endif
