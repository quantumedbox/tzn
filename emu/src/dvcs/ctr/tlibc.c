/*
  Controller device implementation that relies on libc
*/
#include "ttzn.h"

/* Vector of device presence */
static
T_U8 tCtrDVec[] = {
  0x01 /* CTR is always present */
#if defined(T_TRM_D)
 ,0x01
#else
 ,0x00
#endif
#if defined(T_KBD_D)
 ,0x01
#else
 ,0x00
#endif
};

#define T_CTRFx1() tCpuRam[0x02] = tCtrDVec[tCpuRam[0x01]]

#define T_CTRIN() do { \
  tCpuRam[0x00] = T_HOSTID; \
  tCpuRam[0x03] = T_PG_N; \
} while(0)
