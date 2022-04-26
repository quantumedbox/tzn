/*
  Controller device implementation that relies on libc
*/
#include <stdlib.h>

#include "ttzn.h"

static
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
  tCpuRes_();
}

#include "dvcs/tctr.c"
