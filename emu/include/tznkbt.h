#ifndef TZN_KBTH
#define TZN_KBTH

#include "tznstd.h"

#define TZN_KBTR 0x00 /* Reset */
#define TZN_KBTC 0x10 /* Get char */

void tznKbtIn(void);
void tznKbtWr(U8 byte);
U8 tznKbtRd(void);

#endif
