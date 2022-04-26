#include "tzntrm.h"
#include "tcmplr.h"

/* Determines command sequence */
static T_U8 tTrmStat;
/* Used as temporary for character getting */
static T_U8 tTrmLkX;
static T_U8 tTrmLkY;

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
tTrmInit(void)
{
  tTrmStat = tsNone;
  tTrmInIn();
}

T_HOT
void
tznTrmWr(T_U8 byte)
{
  if (byte == TZN_TRMR) {
    tTrmStat = tsNone;
    return;
  }

  switch (tTrmStat) {

    /* Expecting to start new command sequence */
    case tsNone:
    {
      switch (byte)
      {
        case TZN_TRMX:
        {
          tTrmStat = tsWSCurX;
          break;
        }
        case TZN_TRMY:
        {
          tTrmStat = tsWSCurY;
          break;
        }
        case TZN_TRMV:
        {
          tTrmStat = tsWSCurV;
          break;
        }
        case TZN_TRMC:
        {
          tTrmStat = tsWPutCh;
          break;
        }
        case TZN_TRMS:
        {
          tTrmStat = tsWPutSt; /* Wair Put String */
          break;
        }
        case TZN_TRMD:
        {
          tTrmStat = tsSDisSX; /* Send */
          break;
        }
        case TZN_TRMA:
        {
          tTrmStat = tsWChX;  /* Get Char Wait X */
          break;
        }
      }
      break;
    }

    /* Set cursor horizontal pos */
    case tsWSCurX:
    {
      tznTrmCX(byte);
      tTrmStat = tsNone;
      break;
    }

    /* Set cursor vertical pos */
    case tsWSCurY:
    {
      tznTrmCY(byte);
      tTrmStat = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward */
    case tsWSCurV:
    {
      tznTrmCV(byte);
      tTrmStat = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward */
    case tsWPutCh:
    {
      tznTrmPC(byte);
      tTrmStat = tsNone;
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
      tTrmLkX = byte;
      tTrmStat = tsWChY;
      break;
    }

    /* Set temporary for y position, await reading of char */
    case tsWChY:
    {
      tTrmLkY = byte;
      tTrmStat = tsSCh; /* */;
      break;
    }
  }
}

T_U8
tznTrmRd(void)
{
  switch (tTrmStat) {

    /* Send display widtht await to send height */
    case tsSDisSX: /* Send Display Size X */
    {
      tTrmStat = tsSDisSY;
      return tTrmSzX;
    }

    /* Send display height, end command sequence */
    case tsSDisSY:
    {
      tTrmStat = tsNone;
      return tTrmSzY;
    }

    /* Send character at previously specified position, end command sequence */
    case tsSCh    /* */:
    {
      tTrmStat = tsNone;
      return tznTrmGC(tTrmLkX, tTrmLkY);
    }

    /*
      If terminal doesn't expect reading it returns zero byte
      TODO But at the same time nothing says it should? Maybe we could specify that 0x00 is expected to be read
    */
    default:
      return 0x00;
  }
}
