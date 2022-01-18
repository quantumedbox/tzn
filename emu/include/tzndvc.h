#ifndef TZN_DEVICE_H
#define TZN_DEVICE_H

#include "tznstd.h"

void tznDvcIn(void);

TZN_LIKELY void tznDvcWr(U8 byte, U8 device);

TZN_LIKELY U8 tznDvcRd(U8 device);

#endif
