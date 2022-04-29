/*
  DOS compatible keyboard interface
*/

#include <conio.h>

#include "tcmplr.h"

T_U8
tznKbtGC(void)
{
  if (kbhit())
    return (T_U8)getch();
  else
    return 0x00;
}

#include "dvcs/tkbt.c"

static
void
tKbtInit(void)
{
  /* Empty */
}
