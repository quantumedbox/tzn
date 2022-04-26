/*
  Controller is the only device that is guaranteed to be implemented in every runtime
*/

#include "tznctr.h"
#include "tcmplr.h"

/* Write */
static
void
tznCtrWr(void)
{
  switch (tCpuDvIn)
  {
    case TZN_CTRS:
    {
      tznCtrSD();
    }
    case TZN_CTRR:
    {
      tznCtrRS();
    }
    default: (void)0;
  }
}

/* Read */
static
void
tznCtrRd(void)
{
  tCpuDvIn = 0;
}
