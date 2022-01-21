/*
  Commodore 64 compatible terminal device
*/

#include "tznstd.h"
#include "tznsys.h"

#define T_WIDTH 40
#define T_HEIGHT 25

const U8 tznTrmSX = T_WIDTH; /* Width */
const U8 tznTrmSY = T_HEIGHT; /* Height */

static U8 cur_xpos;
static U8 cur_ypos;

static U8* mscr = (U8*)(void*)0x0400;
#define MSCRSIZE 1000
static U8* mcurx = (U8*)(void*)0x00D3;
static U8* mcury = (U8*)(void*)0x00D6;
static U8* mcurv = (U8*)(void*)0x00CC;

/* Translate ASCII to default C64 character set */
static
U8
frASCII(U8 ch)
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
U8
toASCII(U8)
{
  /* TODO Handle all cases somehow */
  if (ch < 0x20)
    return ch + 0x40;
  else
    return ch;
}

TZN_HOT
static
void
updCurXY(void)
{
  *mcurx = cur_xpos;
  *mcury = cur_ypos;
}

void
tznTrmII(void)
{
  cur_xpos = 0;
  cur_ypos = 0;
  tznMeSet(mscr, MSCRSIZE, 0x20);
}

void
tznTrmCX(U8 pos)
{
  while (pos >= tznTrmSX)
    pos -= tznTrmSX;
  cur_xpos = pos;
  updCurXY();
}

void
tznTrmCY(U8 pos)
{
  while (pos >= tznTrmSY)
    pos -= tznTrmSY;
  cur_ypos = pos;
  updCurXY();
}

void
tznTrmCV(U8 state)
{
  /* 0x00 - on, anything else - off */
  *mcurv = !state;
}

TZN_HOT
void
tznTrmPC(U8 ch)
{
  /* TODO Current implementation is suboptimal, but we need to prevent terminal from evaluating newlines and other symbols */
  mscr[cur_xpos + cur_ypos * T_WIDTH] = frASCII(ch);
  if (tznTrmSX == (++cur_xpos))
  {
    cur_xpos = 0;
    if (tznTrmSY == (++cur_ypos))
      cur_ypos = 0;
  }
  updCurXY();
}

U8
tznTrmGC(U8 x, U8 y)
{
  /* TODO Wrap X and Y pos? It would be better for implementation to state that wrapping is underfined */
  return toASCII(mscr[x + y * T_WIDTH]); /* TODO Translate C64 set to ASCII */
}
