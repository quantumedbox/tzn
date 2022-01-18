#ifndef TZN_CONTROLLER_H
#define TZN_CONTROLLER_H

#include "tznstd.h"

#define CONTROLLER_SHUTDOWN 0x01
#define CONTROLLER_RESTART  0x02

void tznCtrIn(void);
void tznCtrWr(U8 byte);
U8 tznCtrRd(void);

#endif
