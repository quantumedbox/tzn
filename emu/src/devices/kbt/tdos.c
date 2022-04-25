/*
  DOS compatible keyboard interface
*/

#include <conio.h>

#include "tznstd.h"

void
tKbtInIn(void)
{
  /* Empty */
}

T_U8
tznKbtGC(void)
{
  if (kbhit())
    return (T_U8)getch();
  else
    return 0x00;
}

#include "tkbt.c"
