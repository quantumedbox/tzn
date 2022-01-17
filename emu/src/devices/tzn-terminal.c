#include "tzn-terminal.h"
#include "tzn-std.h"

static U8 terminal_state;

/* Used as temporary for character getting */
static U8 lookup_x;
static U8 lookup_y;

/* Implementation dependent functions, compile against something that implements them */
extern void tzn_TerminalInitImpl(void);
extern void tzn_TerminalSetCursorHorizontalPos(U8 pos);
extern void tzn_TerminalSetCursorVerticalPos(U8 pos);
extern void tzn_TerminalSetCursorVisibility(U8 state);
extern TZN_LIKELY void tzn_TerminalPutChar(U8 ch);
extern U8 tzn_TerminalGetChar(U8 horizontal, U8 vertical);

extern U8 tzn_terminal_width;
extern U8 tzn_terminal_height;

enum {
  tsNone,
  tsWaitSetCursorHorizontalPos,
  tsWaitSetCursorVerticalPos,
  tsWaitSetCursorVisibility,
  tsWaitPutCharacter,
  tsWaitPutString,
  tsSendDisplayWidth,
  tsSendDisplayHeight,
  tsGetCharWaitHorizontalPos,
  tsGetCharWaitVerticalPos,
  tsGetCharSendChar
};

void
tzn_TerminalInit(void)
{
  terminal_state = tsNone;
  tzn_TerminalInitImpl();
}

TZN_LIKELY
void
tzn_TerminalWrite(U8 byte)
{
  if (byte == TERMINAL_RESET) {
    terminal_state = tsNone;
    return;
  }

  switch (terminal_state) {

    /* Expecting to start new command sequence */
    case tsNone:
    {
      switch (byte)
      {
        case TERMINAL_SET_CURSOR_HORIZONTRAL:
        {
          terminal_state = tsWaitSetCursorHorizontalPos;
          break;
        }
        case TERMINAL_SET_CURSOR_VERTICAL:
        {
          terminal_state = tsWaitSetCursorVerticalPos;
          break;
        }
        case TERMINAL_SET_CURSOR_VISIBILITY:
        {
          terminal_state = tsWaitSetCursorVisibility;
          break;
        }
        case TERMINAL_PUT_CHAR:
        {
          terminal_state = tsWaitPutCharacter;
          break;
        }
        case TERMINAL_PUT_STRING:
        {
          terminal_state = tsWaitPutString;
          break;
        }
        case TERMINAL_GET_DISPLAY_SIZE:
        {
          terminal_state = tsSendDisplayWidth;
          break;
        }
        case TERMINAL_GET_CHAR:
        {
          terminal_state = tsGetCharWaitHorizontalPos;
          break;
        }
      }
      break;
    }

    /* Set cursor horizontal pos */
    case tsWaitSetCursorHorizontalPos:
    {
      tzn_TerminalSetCursorHorizontalPos(byte);
      terminal_state = tsNone;
      break;
    }

    /* Set cursor vertical pos */
    case tsWaitSetCursorVerticalPos:
    {
      tzn_TerminalSetCursorVerticalPos(byte);
      terminal_state = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward */
    case tsWaitSetCursorVisibility:
    {
      tzn_TerminalSetCursorVisibility(byte);
      terminal_state = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward */
    case tsWaitPutCharacter:
    {
      tzn_TerminalPutChar(byte);
      terminal_state = tsNone;
      break;
    }

    /* Print character at cursor position, move cursor forward, wait next character of 0x00 to stop */
    case tsWaitPutString:
    {
      tzn_TerminalPutChar(byte);
      break;
    }

    /* Set temporary for x position, await to send y position */
    case tsGetCharWaitHorizontalPos:
    {
      lookup_x = byte;
      terminal_state = tsGetCharWaitVerticalPos;
      break;
    }

    /* Set temporary for y position, await reading of char */
    case tsGetCharWaitVerticalPos:
    {
      lookup_y = byte;
      terminal_state = tsGetCharSendChar;
      break;
    }
  }
}

U8
tzn_TerminalRead(void)
{
  switch (terminal_state) {

    /* Send display width, await to send height */
    case tsSendDisplayWidth:
    {
      terminal_state = tsSendDisplayHeight;
      return tzn_terminal_width;
    }

    /* Send display height, end command sequence */
    case tsSendDisplayHeight:
    {
      terminal_state = tsNone;
      return tzn_terminal_height;
    }

    /* Send character at previously specified position, end command sequence */
    case tsGetCharSendChar:
    {
      terminal_state = tsNone;
      return tzn_TerminalGetChar(lookup_x, lookup_y);
    }

    /*
      If terminal doesn't expect reading it returns zero byte
      TODO But at the same time nothing says it should? Maybe we could specify that 0x00 is expected to be read
    */
    default:
      return 0x00;
  }
}
