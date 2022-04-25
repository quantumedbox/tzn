#ifndef TZN_CPUH
#define TZN_CPUH

#include "tznstd.h"
#include "tznops.h"

typedef void (*CpuMemCB)(T_U8* memory, T_U16 size);

T_NORET void tznCpuEx(void);

/*
  @brief  Register function that should initialize CPU internal memory on startup
*/
void tznCpuMc(CpuMemCB);

#endif
