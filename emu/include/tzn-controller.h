#ifndef TZN_CONTROLLER_H
#define TZN_CONTROLLER_H

#include "tzn-std.h"

#define CONTROLLER_SHUTDOWN 0x01
#define CONTROLLER_RESTART  0x02

void tzn_ControllerInit(void);
void tzn_ControllerWrite(U8 byte);
U8 tzn_ControllerRead(void);

#endif
