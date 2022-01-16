#include "tzn-terminal.h"
#include "tzn-std.h"

static U8 terminal_state;

enum {
  tsNone,
  tsWaitSetCursorHorizontalPos,
  tsWaitSetCursorVerticalPos,
  tsWaitSetCursorVisibility,
  tsWaitPutCharacter,
  tsWaitPutString,
  tsSendDisplayWidth,
  tsSendDesplayHeight,
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

    /* Initialize sequence */
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

    default:
    {
      tzn_Error("invalid terminal state"); /* TEMP */
    }
  }
}

TZN_UNLIKELY
U8
tzn_TerminalRead(void)
{
  switch (terminal_state) {


    /* If terminal doesn't expect reading it returns zero byte */
    default:
      return 0x00;
  }
}
