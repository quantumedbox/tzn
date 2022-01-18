/*
  VT100 based terminal implementation
*/

#include <stdio.h>

#include "tznstd.h"

#define T_WIDTH 80
#define T_HEIGHT 25

#define ESC_SEQ "\033["
#define HIDE_CUR ESC_SEQ "?25l"
#define SHOW_CUR ESC_SEQ "?25h"
#define CLR_SCR ESC_SEQ "2J"

const U8 tznTrmSX = T_WIDTH; /* Width */
const U8 tznTrmSY = T_HEIGHT; /* Height */

static U8 cur_xpos;
static U8 cur_ypos;

static U8 scr_map[T_WIDTH * T_HEIGHT];

TZN_LIKELY
static
void
updCurXY(void)
{
  fprintf(
    stdout,
    ESC_SEQ "%d;%dH",
    cur_ypos,
    cur_xpos
  );
}

void
tznTrmII(void)
{
  U16 idx;

  cur_xpos = 0;
  cur_ypos = 0;
  updCurXY();

  fputs(CLR_SCR, stdout);

  idx = T_WIDTH * T_HEIGHT;
  while (idx--)
    scr_map[idx] = 0;
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

/* NOTE These sequences aren't part of standard VT100 and might not be available */
void
tznTrmCV(U8 state)
{
  if (state == 0x01)
    fputs(SHOW_CUR, stdout);
  else
    fputs(HIDE_CUR, stdout);
}

TZN_LIKELY
void
tznTrmPC(U8 ch)
{
  /* TODO Current implementation is suboptimal, but we need to prevent terminal from evaluating newlines and other symbols */
  scr_map[cur_xpos + cur_ypos * tznTrmSX] = ch;
  fputc(ch, stdout);
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
  return scr_map[x + y * tznTrmSX];
}
