/*
  DOS compatible way of keyboard char getting
*/

#include <conio.h>

#include "tznstd.h"

void
tznKbtII(void)
{
  /* Empty */
}

U8
tznKbtGC(void)
{
  if (kbhit())
    return (U8)getch();
  else
    return 0x00;
}
