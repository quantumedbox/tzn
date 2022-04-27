/*
  Controller device implementation that relies on libc
*/
#include <stdlib.h>

#include "ttzn.h"

T_NORET
void
tznCtrSD(void)
{
  exit(EXIT_SUCCESS);
}

void
tznCtrRS(void)
{
  
}

#include "dvcs/tctr.c"

static
void
tCtrInit(void)
{
  /* Empty */
}
