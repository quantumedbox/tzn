#include "tznctr.h"

extern void tznCtrII(void); /* Internal init */
extern TZN_NORETURN void tznCtrSD(void); /* Shutdown */
extern void tznCtrRS(void); /* Reset */

void
tznCtrIn(void)
{
  tznCtrII();
}

void
tznCtrWr(U8 byte)
{
  switch (byte)
  {
    case CONTROLLER_SHUTDOWN:
    {
      tznCtrSD();
    }
    case CONTROLLER_RESTART:
    {
      tznCtrRS();
    }
    default:
    {
      return;
    }
  }
}

U8
tznCtrRd(void)
{
  return 0;
}
