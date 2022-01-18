#include "tznstd.h"
#include "tznio.h"

#ifdef TZN_HAS_TERMINAL
#include "tzntrm.h"
#endif

#include "tznctr.h"

/* Ports are dependent on availability of devices, no assumptions should be made about their order */
enum {
  dvNone,
#ifdef TZN_HAS_TERMINAL
  dvTerminal,
#endif
  dvController
};

/* Initializing is setting devices up in their startup state */
void
tznDvcIn(void)
{
#ifdef TZN_HAS_TERMINAL
  tznTrmIn();
#endif
  tznCtrIn();
}

TZN_LIKELY
void
tznDvcWr(U8 byte, U8 device)
{
  switch (device)
  {
#ifdef TZN_HAS_TERMINAL
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

TZN_LIKELY
U8
tznDvcRd(U8 device)
{
  switch (device)
  {
#ifdef TZN_HAS_TERMINAL
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
