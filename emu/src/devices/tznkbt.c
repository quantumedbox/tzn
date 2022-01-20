#include "tznkbt.h"

enum {
  tsNone,
  tsSKey /* Send key */
};

extern U8 tznKbtII(void);

/*
  @brief  Non-blocking get char
          Will return ASCII keycode or 0x00 (NULL) if no key is available
*/
extern U8 tznKbtGC(void);

static U8 state;

/* Internal Initialization */
void
tznKbtIn(void)
{
  tznKbtII();
}

/* Write */
void
tznKbtWr(U8 byte)
{
  switch (byte)
  {
    case TZN_KBTR:
    {
      state = tsNone;
      break;
    }
    case TZN_KBTC:
    {
      state = tsSKey;
      break;
    }
    default:
    {
      return;
    }
  }
}

/* Read */
U8
tznKbtRd(void)
{
  switch (state)
  {
    case tsSKey:
    {
      state = tsNone;
      return tznKbtGC();
    }
    default:
    {
      return 0x00;
    }
  }
}
