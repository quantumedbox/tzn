#ifndef TZN_CPUH
#define TZN_CPUH

#include "tznstd.h"
#include "tznops.h"

typedef void (*CpuMemCB)(U8* memory, U16 size);

void TZN_NORE tznCpuEx(void);

/*
  @brief  Register function that should initialize CPU internal memory on startup
*/
void tznCpuMc(CpuMemCB);

#endif
