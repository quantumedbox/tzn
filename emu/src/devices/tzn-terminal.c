#include "tzn-terminal.h"
#include "tzn-std.h"

static U8 terminal_state;

/* Used as temporary for character getting */
static U8 lookup_x;
static U8 lookup_y;

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
  tsGetCharSendChar,
};

TZN_LIKELY
void
tzn_TerminalWrite(U8 byte)
{
  if (byte == TERMINAL_RESET) {
    terminal_state = tsNone;
    return;
  }

  switch (terminal_state) {

    /* Start command sequence */
    case tsNone:
    {
      switch (byte)
      {
        case TERMINAL_SET_CURSOR_HORIZONTRAL:
        {
          terminal_state = tsWaitSetCursorHorizontalPos;
          return;
        }
        case TERMINAL_SET_CURSOR_VERTICAL:
        {
          terminal_state = tsWaitSetCursorVerticalPos;
          return;
        }
        case TERMINAL_SET_CURSOR_VISIBILITY:
        {
          terminal_state = tsWaitSetCursorVisibility;
          return;
        }
        case TERMINAL_PUT_CHAR:
        {
          terminal_state = tsWaitPutCharacter;
          return;
        }
        case TERMINAL_PUT_STRING:
        {
          terminal_state = tsWaitPutString;
          return;
        }
        case TERMINAL_GET_DISPLAY_SIZE:
        {
          terminal_state = tsSendDisplayWidth;
          return;
        }
        case TERMINAL_GET_CHAR:
        {
          terminal_state = tsGetCharWaitHorizontalPos;
          return;
        }
        default:
          return;
      }
    }

    /* Set cursor horizontal pos */
    case tsWaitSetCursorHorizontalPos:
    {
      tzn_TerminalSetCursorHorizontalPos(byte);
      terminal_state = tsNone;
      return;
    }

    /* Set cursor vertical pos */
    case tsWaitSetCursorVerticalPos:
    {
      tzn_TerminalSetCursorVerticalPos(byte);
      terminal_state = tsNone;
      return;
    }

    /* Print character at cursor position, move cursor forward */
    case tsWaitSetCursorVisibility:
    {
      tzn_TerminalSetCursorVisibility(byte);
      terminal_state = tsNone;
      return;
    }

    /* Print character at cursor position, move cursor forward */
    case tsWaitPutCharacter:
    {
      tzn_TerminalPutChar(byte);
      terminal_state = tsNone;
      return;
    }

    /* Print character at cursor position, move cursor forward, wait next character of 0x00 to stop */
    case tsWaitPutString:
    {
      tzn_TerminalPutChar(byte);
      return;
    }

    /* Set temporary for x position, await to send y position */
    case tsGetCharWaitHorizontalPos:
    {
      lookup_x = byte;
      terminal_state = tsGetCharWaitVerticalPos;
      return;
    }

    /* Set temporary for y position, await reading of char */
    case tsGetCharWaitVerticalPos:
    {
      lookup_y = byte;
      terminal_state = tsGetCharSendChar;
      return;
    }

    default:
    {
      tzn_Error("invalid terminal state"); /* TEMP */
    }
  }
  TZN_UNREACHABLE();
}

TZN_UNLIKELY
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

    /* If terminal doesn't expect reading it returns zero byte */
    default:
      return 0x00;
  }
  TZN_UNREACHABLE();
}
