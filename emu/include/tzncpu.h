#ifndef TZN_CPU_H
#define TZN_CPU_H

#include "tznstd.h"
#include "tznops.h"

typedef void (*CpuMemCB)(U8* memory, U16 size);

/*
  @brief  Enter CPU's execution loop
  @warn   Previous call to tznCpuMc() is required for CPU to have memory initialized
*/
void TZN_NORE tznCpuEx(void);

/*
  @brief  Register function that should initialize CPU internal memory on startup
*/
void tznCpuMc(CpuMemCB);

/*
  @brief  Signal to CPU that it should reset itself and all attached devices
  @warn   Should only be called inside tznCpuEx() running
*/
void tznCpuRs(void);

#endif
