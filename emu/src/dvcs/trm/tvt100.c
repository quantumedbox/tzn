/*
  VT100 based terminal implementation, with output to stdout
*/

#include <sec_api/stdio_s.h>
#include <stdio.h>
#include <string.h>

#include "tcmplr.h"

#define T_TRM_WD (80) /* Default screen width */
#define T_TRM_HG (25) /* Default screen height */

#define T_TRMESQ "\033[" /* Escape sequence */
#define T_TRMHCR T_TRMESQ "?25l" /* Hide cursor */
#define T_TRMSCR T_TRMESQ "?25h" /* Show cursor */
#define T_TRMCLR T_TRMESQ "2J" /* Clear screen */

/* Update cursor position */
#define tTrmUpd() (void)fprintf(stdout, T_TRMESQ"%d;%dH", tCpuRam[0x13] + 1, tCpuRam[0x12] + 1)

/* https://groups.google.com/g/comp.os.vms/c/bDKSY6nG13k?pli=1 */
/* Tho this technique might not behave correctly on some terminal emulators, this needs testing */
static
void
tTrmGetS(void)
{
  /* TODO We could call `tput` process, but it is of limited availability */

  /* Cannot infer the size, assume default */
  /* TODO Could at least use T_HOSTID for inferring */
  tCpuRam[0x10] = T_TRM_WD;
  tCpuRam[0x11] = T_TRM_HG;
}

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
  fflush(stdout);
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

#define T_TRMFx4() (void)fputs(tCpuRam[0x14] ? T_TRMSCR : T_TRMHCR, stdout)
#define T_TRMFx6() tTrmPutC()

#define T_TRMFxF() do { \
  if (tCpuRam[0x1F]) { \
    (void)fputs(T_TRMCLR, stdout); \
    tTrmGetS(); \
  } \
} while (0)
