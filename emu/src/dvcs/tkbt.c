#include "tznkbt.h"

enum {
  tsNone,
  tsSKey /* Send key */
};

static T_U8 tKbtStat;

/* Internal Initialization */
void
tKbtInit(void)
{
  tKbtInIn();
}

/* Write */
void
tznKbtWr(T_U8 byte)
{
  switch (byte)
  {
    case TZN_KBTR:
    {
      tKbtStat = tsNone;
      break;
    }
    case TZN_KBTC:
    {
      tKbtStat = tsSKey;
      break;
    }
    default:
    {
      return;
    }
  }
}

/* Read */
T_U8
tznKbtRd(void)
{
  switch (tKbtStat)
  {
    case tsSKey:
    {
      tKbtStat = tsNone;
      return tznKbtGC();
    }
    default:
    {
      return 0x00;
    }
  }
}
