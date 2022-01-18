#ifndef TZN_CPU_H
#define TZN_CPU_H

#include "tznstd.h"
#include "tznops.h"

typedef void (*CpuMemoryInitCallback)(U8* memory, U16 size);

/* Signal to CPU that it should reset itself and all attached devices */
void tzn_CpuRestart(void);

/* Register function that should initialize CPU internal memory on startup */
void tzn_CpuRegisterMemoryInitCallback(CpuMemoryInitCallback);

void TZN_NORETURN tzn_CpuExec(void);

#endif
