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

static
void
tTrmInit(void)
{
  tTrmStat = tsNone;
  tTrmInIn();
}

static
void
tznTrmWr(void)
{
  if (tCpuDvIn == TZN_TRMR) {
    tTrmStat = tsNone;
    return;
  }

  switch (tTrmStat) {

    /* Expecting to start new command sequence */
    case tsNone:
    {
      switch (tCpuDvIn)
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
      tznTrmCX(tCpuDvIn);
      tTrmStat = tsNone;
      break;
    }

    /* Set cursor vertical pos */
    case tsWSCurY:
    {
      tznTrmCY(tCpuDvIn);
      tTrmStat = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward */
    case tsWSCurV:
    {
      tznTrmCV(tCpuDvIn);
      tTrmStat = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward */
    case tsWPutCh:
    {
      tznTrmPC(tCpuDvIn);
      tTrmStat = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward, wait next character of 0x00 to stop */
    case tsWPutSt: /* Wair Put String */
    {
      tznTrmPC(tCpuDvIn);
      break;
    }

    /* Set temporary for x position, await to send y position */
    case tsWChX:  /* Get Char Wait X */
    {
      tTrmLkX = tCpuDvIn;
      tTrmStat = tsWChY;
      break;
    }

    /* Set temporary for y position, await reading of char */
    case tsWChY:
    {
      tTrmLkY = tCpuDvIn;
      tTrmStat = tsSCh; /* */;
      break;
    }
  }
}

void
tznTrmRd(void)
{
  switch (tTrmStat) {

    /* Send display widtht await to send height */
    case tsSDisSX: /* Send Display Size X */
    {
      tTrmStat = tsSDisSY;
      tCpuDvIn = tTrmSzX;
    }

    /* Send display height, end command sequence */
    case tsSDisSY:
    {
      tTrmStat = tsNone;
      tCpuDvIn = tTrmSzY;
    }

    /* Send character at previously specified position, end command sequence */
    case tsSCh    /* */:
    {
      tTrmStat = tsNone;
      tCpuDvIn = tznTrmGC(tTrmLkX, tTrmLkY);
    }

    default: (void)0;
  }
}
