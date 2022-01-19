/*
  Controller device implementation that relies on libc
*/
#include <stdlib.h>

#include "tznstd.h"
#include "tzncpu.h"
#include "tzndvc.h"

extern void tznCpuRs(void); /* Cpu Reset */

void
tznCtrII(void)
{
  /* Empty */
}

TZN_NORE
void
tznCtrSD(void)
{
  exit(EXIT_SUCCESS);
}

void
tznCtrRS(void)
{
  tznCpuRs();
}
