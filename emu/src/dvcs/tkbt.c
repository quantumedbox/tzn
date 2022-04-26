#include "tznkbt.h"

enum {
  tsNone,
  tsSKey /* Send key */
};

static T_U8 tKbtStat;

/* Internal Initialization */
static
void
tKbtInit(void)
{
  tKbtInIn();
}

/* Write */
static
void
tznKbtWr(void)
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
    default: (void)0;
  }
}

/* Read */
static
void
tznKbtRd(void)
{
  switch (tKbtStat)
  {
    case tsSKey:
    {
      tKbtStat = tsNone;
      tznKbtGC();
    }
    default: (void)0;
  }
}
