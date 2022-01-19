#include "tznctr.h"

extern void tznCtrII(void); /* Internal init */
extern TZN_NORE void tznCtrSD(void); /* Shutdown */
extern void tznCtrRS(void); /* Reset */

/* Internal Initialization */
void
tznCtrIn(void)
{
  tznCtrII();
}

/* Write */
void
tznCtrWr(U8 byte)
{
  switch (byte)
  {
    case TZN_CTRS:
    {
      tznCtrSD();
    }
    case TZN_CTRR:
    {
      tznCtrRS();
    }
    default:
    {
      return;
    }
  }
}

/* Read */
U8
tznCtrRd(void)
{
  return 0;
}
