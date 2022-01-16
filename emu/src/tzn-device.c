#include "tzn-std.h"

#ifdef TZN_HAS_TERMINAL
#include "tzn-terminal.h"
#endif

enum {
  dvNone,
#ifdef TZN_HAS_TERMINAL
  dvTerminal
#endif
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
      break;
    }
#endif
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
    default:
    {
      tzn_Error("invalid device"); /* TEMP */
    }
  }
  TZN_UNREACHABLE();
}
