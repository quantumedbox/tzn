#include "tcmplr.h"
#include "tarch.h"
#include "tio.h"
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
static
void
tznDvcIn(void)
{
  tCtrInit();
#ifdef T_TRM_D
  tTrmInit();
#endif
#ifdef TZN_HKBT
  tKbtInit();
#endif
}

static
void
tznDvcWr(void)
{
  switch (tCpuRgD)
  {
    case dvCtr:
    {
      tznCtrWr();
      break;
    }
#ifdef T_TRM_D
    case dvTrm:
    {
      tznTrmWr();
      break;
    }
#endif
#ifdef TZN_HKBT
    case dvKbt:
    {
      tznKbtWr();
      break;
    }
#endif
    default:
    {
      tError("invalid device"); /* TODO Should be undefined behavior instead? */
    }
  }
}

static
void
tznDvcRd(void)
{
  switch (tCpuRgD)
  {
    case dvCtr:
    {
      tznCtrRd();
    }
#ifdef T_TRM_D
    case dvTrm:
    {
      tznTrmRd();
    }
#endif
#ifdef TZN_HKBT
    case dvKbt:
    {
      tznKbtRd();
    }
#endif
    default:
    {
      tError("invalid device"); /* TODO Should be undefined behavior instead? */
    }
  }
}
