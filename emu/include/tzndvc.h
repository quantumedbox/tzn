#ifndef TZN_DVCH
#define TZN_DVCH

#include "tznstd.h"

TZN_COLD void tznDvcIn(void);

TZN_HOT void tznDvcWr(T_U8 byte, T_U8 device);

TZN_HOT T_U8 tznDvcRd(T_U8 device);

#endif
