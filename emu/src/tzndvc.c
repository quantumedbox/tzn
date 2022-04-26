#include "tcmplr.h"
#include "tarch.h"
#include "tznsys.h"
#include "tasrt.h"

#include T_CTR_D

#ifdef T_TRM_D
#include T_TRM_D
#endif

#ifdef TZN_KBT_D
#include TZN_KBT_D
#endif

/* Ports are dependent on availability of devices, no assumptions should be made about their order */
enum {
   dvCtr
#ifdef T_TRM_D
  ,dvTrm
#endif
#ifdef TZN_HKBT
  ,dvKbt
#endif
};

/* Initializing is setting devices up in their startup state */
T_COLD
void
tznDvcIn(void)
{
#ifdef T_TRM_D
  tTrmInit();
#endif
  tCtrInit();
#ifdef TZN_HKBT
  tKbtInit();
#endif
}

T_HOT
void
tznDvcWr(T_U8 byte, T_U8 device)
{
  switch (device)
  {
#ifdef T_TRM_D
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
      tznError("Invalid device"); /* TEMP */
    }
  }
}

T_HOT
T_U8
tznDvcRd(T_U8 device)
{
  switch (device)
  {
#ifdef T_TRM_D
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
      tznError("Invalid device"); /* TEMP */
    }
  }
}
