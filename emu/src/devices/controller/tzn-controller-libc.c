/*
  Controller device implementation that relies on libc
*/
#include <stdlib.h>

#include "tzn-std.h"
#include "tzn-cpu.h"

TZN_UNLIKELY
TZN_NORETURN
void
tzn_ControllerShutdown(void)
{
  abort();
}

/* TODO We probably need to ensure that nested resets do not cause stack growth */
/* It would probably be better to have CPU signaling ability for that to modify existing loop without invoking new one */
TZN_UNLIKELY
TZN_NORETURN
void
tzn_ControllerReset(void)
{
  tzn_Restart();
  tzn_Exec();
}
