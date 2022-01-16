#ifndef TZN_CONTROLLER_H
#define TZN_CONTROLLER_H

#include "tzn-std.h"

#define CONTROLLER_SHUTDOWN 0x01
#define CONTROLLER_RESTART  0x02

TZN_UNLIKELY void tzn_ControllerWrite(U8 byte);
TZN_UNLIKELY U8 tzn_ControllerRead(void);

extern TZN_NORETURN TZN_UNLIKELY void tzn_ControllerShutdown(void);
extern TZN_NORETURN TZN_UNLIKELY void tzn_ControllerReset(void);

#endif
