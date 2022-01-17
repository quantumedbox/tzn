#include "tznctr.h"

extern void tzn_ControllerInitImpl(void);
extern TZN_NORETURN void tzn_ControllerShutdown(void);
extern void tzn_ControllerReset(void);

void
tzn_ControllerInit(void)
{
  tzn_ControllerInitImpl();
}

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
}

U8
tzn_ControllerRead(void)
{
  return 0;
}
