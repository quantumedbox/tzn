/*
  Commodore 64 keyboard interface
*/

#include "tcmplr.h"

static T_U16* mkport1 = (T_U16*)(void*)0xDC00;
static T_U16* mkport2 = (T_U16*)(void*)0xDC01;

/* https://sta.c64.org/cbm64kbdlay.html */
/* TODO Map non-ascii keys */
T_U8 mtxASCII[][] = {
  {
    0x00, /* Insert/Delete */
    0x00, /* Return */
    0x00, /* Cursor left/right */
    0x00, /* F7 */
    0x00, /* F1 */
    0x00, /* F3 */
    0x00, /* F5 */
    0x00, /* Cursor up/down */
  },
  {
    0x33, /* 3 */
    0x57, /* W */
    0x41, /* A */
    0x34, /* 4 */
    0x5A, /* Z */
    0x53, /* S */
    0x45, /* E */
    0x00, /* Left Shift */
  },
};

void
tKbtInIn(void)
{
  /* Empty */
}

T_U8
tznKbtGC(void)
{
  /* TODO */
  return mtxASCII[*mkport1][*mkport2];
}

#include "dvcs/tkbt.c"
