#include "tzn-std.h"

#ifdef TZN_HAS_TERMINAL
#include "tzn-terminal.h"
#endif

#include "tzn-controller.h"

enum {
  dvNone,
#ifdef TZN_HAS_TERMINAL
  dvTerminal,
#endif
  dvController,
};

TZN_UNLIKELY
void
tzn_DevicesInit(void)
{
#ifdef TZN_HAS_TERMINAL
  tzn_TerminalInit();
#endif
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
      return;
    }
#endif
    case dvController:
    {
      tzn_ControllerWrite(byte);
      return;
    }
    default:
    {
      tzn_Error("invalid device"); /* TEMP */
    }
  }
  TZN_UNREACHABLE();
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
  TZN_UNREACHABLE();
}
