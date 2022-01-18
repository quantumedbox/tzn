/*
  Controller device implementation that relies on libc
*/
#include <stdlib.h>

#include "tznstd.h"
#include "tzncpu.h"
#include "tzndvc.h"

void
tzn_ControllerInitImpl(void)
{
  /* Empty */
}

TZN_NORETURN
void
tzn_ControllerShutdown(void)
{
  exit(EXIT_SUCCESS);
}

/* TODO We probably need to ensure that nested resets do not cause stack growth */
/* It would probably be better to have CPU signaling ability for that to modify existing loop without invoking new one */
/* TODO Reset device state */

void
tzn_ControllerReset(void)
{
  tzn_CpuRestart();
}
