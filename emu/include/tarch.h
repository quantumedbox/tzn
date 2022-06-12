/*
  Specifies implementation details for referred target
  Serves as kind of build system that is encapsulated without C itself

  # Required definitions
    - T_CPU_C -- Specifies CPU source file
    - T_DVCS_C -- Specifies devices source file
    - T_ARCH -- Holds some value of arch enum that is defined in this file, used for identity
    - T_PG_N -- How many "pages" of memory are available for reading and writing
    - T_CTR_D -- Specifies controller device implementation

  # Possible definitions
    - T_CMDARG -- Specifies that argc and argv are present // todo: Use T_MODE=CLI instead
    - T_IO_C -- Specifies IO implementation
    - T_TRM_D -- Specifies terminal device implementation
    - T_KBT_D -- Specifies ASCII keyboard device implementation
*/

#if !defined(TARCH_H)
#define TARCH_H

#define taSelf  (0x00)
#define taMSDOS (0x01)
#define taC64   (0x02)

#define T_PG_SZ (256U) /* Page size in bytes */

#define T_CPU_C "tcpu.c"
#define T_DVCS_C "tdvcs.c"

#define TZN_LEND /* Little Endian is assumed by default, Big Endian targets should undefine it */

#if defined(__C64__)
  /* Commodore 64 */
  #define T_HOSTID taC64
  #define T_PG_N (177U)
  #define T_CTR_D "dvcs/ctr/tlibc.c"
  #define T_TRM_D "dvcs/trm/tc64.c"
  #define T_KBT_D "dvcs/kbt/tc64.c"
#elif defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__)
  /* MS DOS */
  #define T_HOSTID taMSDOS
  #define T_PG_N (128U)
  #define T_IO_C "tio.c"
  #define T_CMDARG
  #define T_CTR_D "dvcs/ctr/tlibc.c"
  #define T_TRM_D "dvcs/trm/tvt100.c"
  #define T_KBT_D "dvcs/kbt/tdos.c"
#else
  /* If no file is routed - "modern" OS hosted PC is assumed, aka what's supposedly native for computer on which this is compiled */
  #define T_HOSTID taSelf
  #define T_PG_N (255U)
  #define T_IO_C "tio.c"
  #define T_CMDARG
  #define T_CTR_D "dvcs/ctr/tlibc.c"
  #define T_TRM_D "dvcs/trm/tvt100.c"
  #if defined(_WIN32)
    #define T_KBT_D "dvcs/kbt/tdos.c"
  #endif
#endif

#if defined(T_IO_C)
#include T_IO_C
#endif

#endif
