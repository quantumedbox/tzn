#ifndef TZN_CPU_H
#define TZN_CPU_H

#include "tznstd.h"
#include "tznops.h"

typedef void (*CpuMemCB)(U8* memory, U16 size);

/* Signal to CPU that it should reset itself and all attached devices */
void tznCpuRs(void);

/* Register function that should initialize CPU internal memory on startup */
void tznCpuMc(CpuMemCB);

void TZN_NORETURN tznCpuEx(void);

#endif
