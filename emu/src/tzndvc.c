#include "tznstd.h"
#include "tznsys.h"
#include "tznerr.h"

#ifdef TZN_HTRM
#include "tzntrm.h"
#endif

#include "tznctr.h"

#ifdef TZN_HKBT
#include "tznkbt.h"
#endif

/* Ports are dependent on availability of devices, no assumptions should be made about their order */
enum {
  dvNone
#ifdef TZN_HTRM
  ,dvTrm
#endif
  ,dvCtr
#ifdef TZN_HKBT
  ,dvKbt
#endif
};

/* Initializing is setting devices up in their startup state */
void
tznDvcIn(void)
{
#ifdef TZN_HTRM
  tznTrmIn();
#endif
  tznCtrIn();
#ifdef TZN_HKBT
  tznKbtIn();
#endif
}

TZN_HOT
void
tznDvcWr(U8 byte, U8 device)
{
  switch (device)
  {
#ifdef TZN_HTRM
    case dvTrm:
    {
      tznTrmWr(byte);
      break;
    }
#endif
    case dvCtr:
    {
      tznCtrWr(byte);
      break;
    }
#ifdef TZN_HKBT
    case dvKbt:
    {
      tznKbtWr(byte);
      break;
    }
#endif
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
    case dvTrm:
    {
      return tznTrmRd();
    }
#endif
    case dvCtr:
    {
      return tznCtrRd();
    }
#ifdef TZN_HKBT
    case dvKbt:
    {
      return tznKbtRd();
    }
#endif
    default:
    {
      tznError("invalid device"); /* TEMP */
    }
  }
}
