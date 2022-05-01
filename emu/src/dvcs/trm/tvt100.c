/*
  VT100 based terminal implementation, with output to stdout
*/

#include <stdio.h>

#include "tcmplr.h"

#define T_TRM_WD 80 /* Screen width */
#define T_TRM_HG 25 /* Screen height */

#define T_TRMESQ "\033[" /* Escape sequence */
#define T_TRMHCR T_TRMESQ "?25l" /* Hide cursor */
#define T_TRMSCR T_TRMESQ "?25h" /* Show cursor */
#define T_TRMCLR T_TRMESQ "2J" /* Clear screen */

/* Update cursor position */
#define tTrmUpd() fprintf(stdout, T_TRMESQ"%d;%dH", tCpuRam[0x13] + 1, tCpuRam[0x12] + 1)

void
tTrmPutC(void)
{
  /* TODO Current implementation is suboptimal, but we need to prevent terminal from evaluating newlines and other symbols */
  fputc(tCpuRam[0x16], stdout);
  if (T_TRM_WD == (++tCpuRam[0x12])) {
    tCpuRam[0x12] = 0;
    if (T_TRM_HG == (++tCpuRam[0x13]))
      tCpuRam[0x13] = 0;
  }
  /* TODO We might need to flush stdout here? */
  tTrmUpd(); /* TODO It shouldn't need to update positions each time */
}

/* TODO Doesn't seem like a good idea, hm */
void
tTrmPrLt(const char* literal)
{
  while (*literal) {
    tCpuRam[0x16] = *literal;
    tTrmPutC();
    ++literal;
  }
}

#define T_TRMFx2() do { \
  tCpuRam[0x12] %= T_TRM_WD; \
  tTrmUpd(); \
} while (0)

#define T_TRMFx3() do { \
  tCpuRam[0x13] %= T_TRM_HG; \
  tTrmUpd(); \
} while (0)

#define T_TRMFx4() fputs(tCpuRam[0x14] ? T_TRMSCR : T_TRMHCR, stdout)
#define T_TRMFx6() tTrmPutC()

/* Initialization */
#define T_TRMIN() do { \
  tCpuRam[0x12] = 0; \
  tCpuRam[0x13] = 0; \
  tTrmUpd(); \
  fputs(T_TRMCLR, stdout); \
} while (0)
