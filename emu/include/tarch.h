/*
  Specifies implementation details for referred target

  # Required definitions
    - T_ARCH -- Holds some value of arch enum that is defined in this file, used for identity
    - T_PG_N -- How many "pages" of memory are available for reading and writing
    - T_CTR_D -- Specifies controller device implementation

  # Possible definitions
    - T_FLSYS -- Specifies that file system is available
    - T_TRM_D -- Specifies terminal device implementation
    - T_KBT_D -- Specifies ASCII keyboard device implementation
*/

#ifndef TARCH_H
#define TARCH_H

enum {
  taSelf,
  taC64
};

#define T_PG_SZ 256 /* Page size in bytes */

#if __CC65__ && __C64__
  #define T_ARCH taC64
  #define T_PG_N 127
  #define T_CTR_D "ctr/tlibc.c"
  #define T_TRM_D "trm/tc64.c"
  #define T_KBT_D "kbt/tc64.c"
#else
  /* If no file is routed - "modern" OS hosted PC is assumed, aka what's supposedly native for computer on which this is compiled */
  #define T_ARCH taSelf
  #define T_PG_N 256
  #define T_FLSYS
  #define T_CTR_D "ctr/tlibc.c"
  #define T_TRM_D "trm/tvt100.c"
  #ifdef _WIN32
    #define T_KBT_D "kbt/tdos.c"
  #endif
  /* TODO: Detect DOS */
#endif

#define T_SPINIT (T_PG_N * T_PG_SZ) /* Initial value of stack pointer */

#endif
