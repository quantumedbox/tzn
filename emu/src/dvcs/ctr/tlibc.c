/*
  Controller device implementation that relies on libc
*/
#include <stdlib.h>

#include "tcmplr.h"
#include "tzncpu.h"
#include "tzndvc.h"

extern void tznCpuRs(void); /* Cpu Reset, defined in tzncpu.c */

void
tCtrInIn(void)
{
  /* Empty */
}

T_NORET
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

#include "dvcs/tctr.c"
