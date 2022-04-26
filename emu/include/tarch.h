/*
  Specifies implementation details for referred target

  # Required definitions
    - T_CPU_C -- Specifies CPU source file
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

#define T_PG_SZ 256U /* Page size in bytes */

#define T_CPU_C "tcpu.c"

#if __CC65__ && __C64__
  #define T_ARCH taC64
  #define T_PG_N 160U
  #define T_CTR_D "dvcs/ctr/tlibc.c"
  #define T_TRM_D "dvcs/trm/tc64.c"
  #define T_KBT_D "dvcs/kbt/tc64.c"
#else
  /* If no file is routed - "modern" OS hosted PC is assumed, aka what's supposedly native for computer on which this is compiled */
  #define T_ARCH taSelf
  #define T_PG_N 256U
  #define T_FLSYS
  #define T_CTR_D "dcvs/ctr/tlibc.c"
  #define T_TRM_D "dvcs/trm/tvt100.c"
  #ifdef _WIN32
    #define T_KBT_D "dvcs/kbt/tdos.c"
  #endif
  /* TODO: Detect DOS */
#endif

#define T_SPINIT (T_PG_N * T_PG_SZ) /* Initial value of stack pointer */

#endif
