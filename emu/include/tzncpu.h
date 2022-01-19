#ifndef TZN_CPU_H
#define TZN_CPU_H

#include "tznstd.h"
#include "tznops.h"

typedef void (*CpuMemCB)(U8* memory, U16 size);

void TZN_NORE tznCpuEx(void);

/* Register function that should initialize CPU internal memory on startup */
void tznCpuMc(CpuMemCB);

/* Signal to CPU that it should reset itself and all attached devices */
void tznCpuRs(void);

#endif
