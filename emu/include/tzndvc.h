#ifndef TZN_DVCH
#define TZN_DVCH

#include "tcmplr.h"

T_COLD void tznDvcIn(void);

T_HOT void tznDvcWr(T_U8 byte, T_U8 device);

T_HOT T_U8 tznDvcRd(T_U8 device);

#endif
