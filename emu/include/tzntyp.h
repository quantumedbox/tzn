#ifndef TZN_TYPH
#define TZN_TYPH

#include "tarch.h"

#define T_NULL    ((void*)0)

#define T_U8      unsigned char
#define T_U16     unsigned short

#define T_I8      signed char
#define T_I16     signed short

#define T_U8_MIN  ((T_U8)0)
#define T_U8_MAX  ((T_U8)-1)

#define T_U16MIN  ((T_U16)0)
#define T_U16MAX  ((T_U16)-1)

#if T_HOSTID == taMSDOS
    #define T_UPTR sizeof(int)
#else
    /*
        Unsigned integer representation of pointer types
        TODO It is from C99 standard, which is not as portable as I would like
    */
    #define T_UPTR intptr_t
#endif

#define T_ERR     T_U8
#define T_BOOL    T_U8
#define T_TRUE    ((T_BOOL)1)
#define T_FALSE   ((T_BOOL)0)

#endif
