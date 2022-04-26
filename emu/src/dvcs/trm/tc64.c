/*
  Commodore 64 compatible terminal device
*/

#include "tcmplr.h"
#include "tio.h"

#define T_TRM_WD 40
#define T_SCR_SZ 1000

const T_U8 tTrmSzX = T_TRM_WD; /* Width */
const T_U8 tTrmSzY = 25; /* Height */

static T_U8 tTrmPosX;
static T_U8 tTrmPosY;

static T_U8* tTrmScrM = (T_U8*)(void*)0x0400;
static T_U8* tTrmXM = (T_U8*)(void*)0x00D3;
static T_U8* tTrmYM = (T_U8*)(void*)0x00D6;
static T_U8* tTrmVM = (T_U8*)(void*)0x00CC;

/* Translate ASCII to default C64 character set */
static
T_U8
frASCII(T_U8 ch)
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
toASCII(T_U8 ch)
{
  /* TODO Handle all cases somehow */
  if (ch < 0x20)
    return ch + 0x40;
  else
    return ch;
}

T_HOT
static
void
updCurXY(void)
{
  *tTrmXM = tTrmPosX;
  *tTrmYM = tTrmPosY;
}

void
tznTrmCX(T_U8 pos)
{
  while (pos >= tTrmSzX)
    pos -= tTrmSzX;
  tTrmPosX = pos;
  updCurXY();
}

void
tznTrmCY(T_U8 pos)
{
  while (pos >= tTrmSzY)
    pos -= tTrmSzY;
  tTrmPosY = pos;
  updCurXY();
}

void
tznTrmCV(T_U8 state)
{
  /* 0x00 - on, anything else - off */
  *tTrmVM = !state;
}

T_HOT
void
tznTrmPC(T_U8 ch)
{
  /* TODO Current implementation is suboptimal, but we need to prevent terminal from evaluating newlines and other symbols */
  tTrmScrM[tTrmPosX + tTrmPosY * T_TRM_WD] = frASCII(ch);
  if (tTrmSzX == (++tTrmPosX))
  {
    tTrmPosX = 0;
    if (tTrmSzY == (++tTrmPosY))
      tTrmPosY = 0;
  }
  updCurXY();
}

T_U8
tznTrmGC(T_U8 x, T_U8 y)
{
  /* TODO Wrap X and Y pos? It would be better for implementation to state that wrapping is underfined */
  return toASCII(tTrmScrM[x + y * T_TRM_WD]); /* TODO Translate C64 set to ASCII */
}

#include "dvcs/ttrm.c" /* General device impl */

static
void
tTrmInit(void)
{
  tTrmStat = tsNone;
  tTrmPosX = 0;
  tTrmPosY = 0;
  tMemSet(tTrmScrM, 0x20, T_SCR_SZ);
}
