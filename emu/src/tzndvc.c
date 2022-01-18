#include "tznstd.h"
#include "tznsys.h"
#include "tznerr.h"

#ifdef TZN_HTRM
#include "tzntrm.h"
#endif

#include "tznctr.h"

/* Ports are dependent on availability of devices, no assumptions should be made about their order */
enum {
  dvNone,
#ifdef TZN_HTRM
  dvTerminal,
#endif
  dvController
};

/* Initializing is setting devices up in their startup state */
void
tznDvcIn(void)
{
#ifdef TZN_HTRM
  tznTrmIn();
#endif
  tznCtrIn();
}

TZN_HOT
void
tznDvcWr(U8 byte, U8 device)
{
  switch (device)
  {
#ifdef TZN_HTRM
    case dvTerminal:
    {
      tznTrmWr(byte);
      break;
    }
#endif
    case dvController:
    {
      tznCtrWr(byte);
      break;
    }
    default:
    {
      tznError("invalid device"); /* TEMP */
    }
  }
}

TZN_HOT
U8
tznDvcRd(U8 device)
{
  switch (device)
  {
#ifdef TZN_HTRM
    case dvTerminal:
    {
      return tznTrmRd();
    }
#endif
    case dvController:
    {
      return tznCtrRd();
    }
    default:
    {
      tznError("invalid device"); /* TEMP */
    }
  }
}
