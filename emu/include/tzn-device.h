#ifndef TZN_DEVICE_H
#define TZN_DEVICE_H

#include "tzn-std.h"

void tzn_DevicesInit(void);

TZN_LIKELY void tzn_DeviceWrite(U8 byte, U8 device);

TZN_LIKELY U8 tzn_DeviceRead(U8 device);

#endif
