#ifndef TZN_CPU_H
#define TZN_CPU_H

#include "tznstd.h"
#include "tznops.h"

void tzn_Restart(void);

void tzn_CpuPassInitMemory(const U8* to_cpy, U16 size);

void TZN_NORETURN tzn_CpuExec(void);

#endif
