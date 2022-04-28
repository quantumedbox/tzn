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

/*
  TODO What about interrupt device?
  For supporting something in the lines of http://6502.org/source/kernels/minikernel.txt
  Might be quite problematic to make it portable tho
  Also, it should be possible to implement scheduler purely in TZN with available timer at this point
*/

/*
  Ports are hardcoded and particular device always is at the same memory address no mater the implementation
  Order of them matters as dispatching might be done via sequence of if-else branches, so, the most used devices should come first
  Controller is always assumed to be the last device
*/
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
tDvcInit(void)
{
  tCtrInit();
#ifdef T_TRM_D
  tTrmInit();
#endif
#ifdef TZN_HKBT
  tKbtInit();
#endif
}

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

void
tznDvcRd(void)
{
  switch (tCpuRgD)
  {
    case dvCtr:
    {
      tznCtrRd();
      break;
    }
#ifdef T_TRM_D
    case dvTrm:
    {
      tznTrmRd();
      break;
    }
#endif
#ifdef TZN_HKBT
    case dvKbt:
    {
      tznKbtRd();
      break;
    }
#endif
    default:
    {
      tError("invalid device"); /* TODO Should be undefined behavior instead? */
    }
  }
}
