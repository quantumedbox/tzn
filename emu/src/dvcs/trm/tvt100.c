/*
  VT100 based terminal implementation, with output to stdout
*/

#include <stdio.h>

#include "tcmplr.h"

#define T_TRM_WD 80 /* Screen width */
#define T_TRM_HG 25 /* Screen height */

#define T_ESC_SEQ "\033["
#define T_HIDE_CUR T_ESC_SEQ "?25l"
#define T_SHOW_CUR T_ESC_SEQ "?25h"
#define T_CLR_SCR T_ESC_SEQ "2J"

const T_U8 tTrmSzX = T_TRM_WD; /* Screen width */
const T_U8 tTrmSzY = T_TRM_HG; /* Screen height */

static T_U8 tTrmPosX;
static T_U8 tTrmPosY;

static T_U8 tTrmScr[T_TRM_WD * T_TRM_HG];

T_HOT
static
void
updCurXY(void)
{
  fprintf(
    stdout,
    T_ESC_SEQ "%d;%dH",
    tTrmPosY + 1,
    tTrmPosX + 1
  );
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

/* NOTE These sequences aren't part of standard VT100 and might not be available */
void
tznTrmCV(T_U8 state)
{
  if (state == 0x01)
    fputs(T_SHOW_CUR, stdout);
  else
    fputs(T_HIDE_CUR, stdout);
}

T_HOT
void
tznTrmPC(T_U8 ch)
{
  /* TODO Current implementation is suboptimal, but we need to prevent terminal from evaluating newlines and other symbols */
  tTrmScr[tTrmPosX + tTrmPosY * tTrmSzX] = ch;
  fputc(ch, stdout);
  if (tTrmSzX == (++tTrmPosX))
  {
    tTrmPosX = 0;
    if (tTrmSzY == (++tTrmPosY))
      tTrmPosY = 0;
  }
  updCurXY(); /* TODO It shouldn't need to update positions each time */
}

T_U8
tznTrmGC(T_U8 x, T_U8 y)
{
  /* TODO Wrap X and Y pos? It would be better for implementation to state that wrapping is underfined */
  return tTrmScr[x + y * tTrmSzX];
}

#include "dvcs/ttrm.c"

static
void
tTrmInit(void)
{
  T_U16 idx;
  tTrmStat = tsNone;

  tTrmPosX = 0;
  tTrmPosY = 0;
  updCurXY();

  fputs(T_CLR_SCR, stdout);

  idx = T_TRM_WD * T_TRM_HG;
  while (idx--)
    tTrmScr[idx] = 0;
}
