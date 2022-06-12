/*
  Commodore 64 compatible terminal device
*/

#include "tcmplr.h"
#include "tio.h"

#define T_TRM_WD (T_U8)40U /* Width */
#define T_TRM_HG (T_U8)25U /* Height */
#define T_SCR_SZ 1000U /* Screen buffer size */

static T_U8* tTrmScrM = (T_U8*)(void*)0x0400;
static T_U8* tTrmXM = (T_U8*)(void*)0x00D3;
static T_U8* tTrmYM = (T_U8*)(void*)0x00D6;
static T_U8* tTrmVM = (T_U8*)(void*)0x00CC;

/* Translate ASCII to default C64 character set */
/* TODO Inline it */
static
T_U8
tTrmFrAs(T_U8 ch)
{
  /* TODO Handle all cases somehow */
  if ((ch >= 0x40) && (ch < 0x60))
    return ch - 0x40;
  else if ((ch >= 0x60))
    return ch - 0x60;
  else
    return ch;
}

static
T_U8
tTrmToAs(T_U8 ch)
{
  /* TODO Handle all cases somehow */
  if (ch < 0x20)
    return ch + 0x40;
  else
    return ch;
}

static
void
tTrmPutC(void)
{
  tTrmScrM[tCpuRam[0x12] + tCpuRam[0x13] * T_TRM_WD] = tTrmFrAs(tCpuRam[0x16]);
  if (T_TRM_WD == (++tCpuRam[0x12])) {
    tCpuRam[0x12] = 0;
    if (T_TRM_HG == (++tCpuRam[0x13]))
      tCpuRam[0x13] = 0;
    *tTrmYM = tCpuRam[0x13];
  }
  *tTrmXM = tCpuRam[0x12];
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

/* TODO Should we wrap in such way? Cropping it might be enough */
#define T_TRMFx2() do { \
  tCpuRam[0x12] %= T_TRM_WD; \
  *tTrmXM = tCpuRam[0x12]; \
} while (0)

/* TODO Should we wrap in such way? Cropping it might be enough */
#define T_TRMFx3() do { \
  tCpuRam[0x13] %= T_TRM_HG; \
  *tTrmYM = tCpuRam[0x13]; \
} while (0)

#define T_TRMFx4() *tTrmVM = !tCpuRam[0x14] /* 0x00 - on, anything else - off */
#define T_TRMFx6() tTrmPutC()

#define T_TRMFxF() do { \
  if (tCpuRam[0x1F]) { \
    tCpuRam[0x10] = T_TRM_WD; \
    tCpuRam[0x11] = T_TRM_HG; \
    tMemSet(tTrmScrM, 0x20, T_SCR_SZ); \
  } \
} while (0)
