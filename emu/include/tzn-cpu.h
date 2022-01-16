#ifndef TZN_CPU_H
#define TZN_CPU_H

#include "tzn-std.h"
#include "tzn-opcodes.h"

void tzn_Restart(void);

void tzn_WriteMemory(const U8* to_cpy, U16 offset, U16 size);

void TZN_NORETURN tzn_Exec(void);

#endif
