#include "tzn-controller.h"

TZN_UNLIKELY
void
tzn_ControllerWrite(U8 byte)
{
  switch (byte)
  {
    case CONTROLLER_SHUTDOWN:
    {
      tzn_ControllerShutdown();
    }
    case CONTROLLER_RESTART:
    {
      tzn_ControllerReset();
    }
    default:
    {
      return;
    }
  }
  TZN_UNREACHABLE();
}

TZN_UNLIKELY
U8
tzn_ControllerRead(void)
{
  return 0;
}
