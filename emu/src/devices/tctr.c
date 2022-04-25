/*
  Controller is the only device that is guaranteed to be implemented in every runtime
*/

#include "tznctr.h"
#include "tznstd.h"

/* Internal Initialization */
void
tCtrInit(void)
{
  tCtrInIn();
}

/* Write */
void
tznCtrWr(T_U8 byte)
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
T_U8
tznCtrRd(void)
{
  return 0;
}
