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
tzn_DevicesInit(void)
{
#ifdef TZN_HAS_TERMINAL
  tzn_TerminalInit();
#endif
  tzn_ControllerInit();
}

TZN_LIKELY
void
tzn_DeviceWrite(U8 byte, U8 device)
{
  switch (device)
  {
#ifdef TZN_HAS_TERMINAL
    case dvTerminal:
    {
      tzn_TerminalWrite(byte);
      break;
    }
#endif
    case dvController:
    {
      tzn_ControllerWrite(byte);
      break;
    }
    default:
    {
      tzn_Error("invalid device"); /* TEMP */
    }
  }
}

TZN_LIKELY
U8
tzn_DeviceRead(U8 device)
{
  switch (device)
  {
#ifdef TZN_HAS_TERMINAL
    case dvTerminal:
    {
      return tzn_TerminalRead();
    }
#endif
    case dvController:
    {
      return tzn_ControllerRead();
    }
    default:
    {
      tzn_Error("invalid device"); /* TEMP */
    }
  }
}
