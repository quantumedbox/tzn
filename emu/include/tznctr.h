#ifndef TZN_CTRH
#define TZN_CTRH

#include "tznstd.h"

#define TZN_CTRS 0x01 /* Shutdown Command */
#define TZN_CTRR 0x02 /* Restart Command */

void tznCtrIn(void);
void tznCtrWr(U8 byte);
U8 tznCtrRd(void);

#endif
