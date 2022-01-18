#include "tzntrm.h"
#include "tznstd.h"

static U8 state;

/* Used as temporary for character getting */
static U8 lookup_x;
static U8 lookup_y;

/* Implementation dependent functions, compile against something that implements them */
extern void tznTrmII(void); /* Internal init */
extern void tznTrmCX(U8 pos); /* Set cursor X pos */
extern void tznTrmCY(U8 pos); /* Set cursor Y pos */
extern void tznTrmCV(U8 state); /* Set cursor visibility */
extern TZN_LIKELY void tznTrmPC(U8 ch); /* Put char */
extern U8 tznTrmGC(U8 x, U8 y); /* Get char */

extern U8 tznTrmSX;
extern U8 tznTrmSY;

enum {
  tsNone,
  tsWSCurX, /* Wait Set Cursor X */
  tsWSCurY, /* Wait Set Cursor Y */
  tsWSCurV, /* Wait Set Cursor visibility */
  tsWPutCh, /* Wait Put Char */
  tsWPutSt, /* Wait Put String */
  tsSDisSX, /* Send Display Size X */
  tsSDisSY, /* Send Display Size Y */
  tsWChX,   /* Wait Char X */
  tsWChY,   /* Wait Char Y */
  tsSCh     /* Send Char */
};

void
tznTrmIn(void)
{
  state = tsNone;
  tznTrmII();
}

TZN_LIKELY
void
tznTrmWr(U8 byte)
{
  if (byte == TERMINAL_RESET) {
    state = tsNone;
    return;
  }

  switch (state) {

    /* Expecting to start new command sequence */
    case tsNone:
    {
      switch (byte)
      {
        case TERMINAL_SET_CURSOR_HORIZONTRAL:
        {
          state = tsWSCurX;
          break;
        }
        case TERMINAL_SET_CURSOR_VERTICAL:
        {
          state = tsWSCurY;
          break;
        }
        case TERMINAL_SET_CURSOR_VISIBILITY:
        {
          state = tsWSCurV;
          break;
        }
        case TERMINAL_PUT_CHAR:
        {
          state = tsWPutCh;
          break;
        }
        case TERMINAL_PUT_STRING:
        {
          state = tsWPutSt; /* Wair Put String */
          break;
        }
        case TERMINAL_GET_DISPLAY_SIZE:
        {
          state = tsSDisSX; /* Send */
          break;
        }
        case TERMINAL_GET_CHAR:
        {
          state = tsWChX;  /* Get Char Wait X */
          break;
        }
      }
      break;
    }

    /* Set cursor horizontal pos */
    case tsWSCurX:
    {
      tznTrmCX(byte);
      state = tsNone;
      break;
    }

    /* Set cursor vertical pos */
    case tsWSCurY:
    {
      tznTrmCY(byte);
      state = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward */
    case tsWSCurV:
    {
      tznTrmCV(byte);
      state = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward */
    case tsWPutCh:
    {
      tznTrmPC(byte);
      state = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward, wait next character of 0x00 to stop */
    case tsWPutSt: /* Wair Put String */
    {
      tznTrmPC(byte);
      break;
    }

    /* Set temporary for x position, await to send y position */
    case tsWChX:  /* Get Char Wait X */
    {
      lookup_x = byte;
      state = tsWChY;
      break;
    }

    /* Set temporary for y position, await reading of char */
    case tsWChY:
    {
      lookup_y = byte;
      state = tsSCh; /* */;
      break;
    }
  }
}

U8
tznTrmRd(void)
{
  switch (state) {

    /* Send display widtht await to send height */
    case tsSDisSX: /* Send Display Size X */
    {
      state = tsSDisSY;
      return tznTrmSX;
    }

    /* Send display height, end command sequence */
    case tsSDisSY:
    {
      state = tsNone;
      return tznTrmSY;
    }

    /* Send character at previously specified position, end command sequence */
    case tsSCh    /* */:
    {
      state = tsNone;
      return tznTrmGC(lookup_x, lookup_y);
    }

    /*
      If terminal doesn't expect reading it returns zero byte
      TODO But at the same time nothing says it should? Maybe we could specify that 0x00 is expected to be read
    */
    default:
      return 0x00;
  }
}
