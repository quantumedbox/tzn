/*
  VT100 terminal implementation
*/

#include <stdio.h>

#include "tzn-std.h"

static U8 terminal_cursor_horizontal_pos;
static U8 terminal_cursor_vertical_pos;
// static U8 terminal_cursor_visibility;

U8 tzn_terminal_width = 80;
U8 tzn_terminal_height = 25;

#define ESCAPE_SEQUENCE "\e["
#define HIDE_CURSOR ESCAPE_SEQUENCE "?25l"
#define SHOW_CURSOR ESCAPE_SEQUENCE "?25h"

TZN_LIKELY
static
void
tzn_UpdateCursorPos(void)
{
  fprintf(
    stdout,
    ESCAPE_SEQUENCE "%d;%dH",
    terminal_cursor_vertical_pos,
    terminal_cursor_horizontal_pos
  );
}

TZN_UNLIKELY
void
tzn_TerminalInit(void)
{
  tzn_UpdateCursorPos();
}

void
tzn_TerminalSetCursorHorizontalPos(U8 pos)
{
  while (pos >= tzn_terminal_width)
    pos -= tzn_terminal_width;
  terminal_cursor_horizontal_pos = pos;
  tzn_UpdateCursorPos();
}

void
tzn_TerminalSetCursorVerticalPos(U8 pos)
{
  while (pos >= tzn_terminal_height)
    pos -= tzn_terminal_height;
  terminal_cursor_vertical_pos = pos;
  tzn_UpdateCursorPos();
}

/* NOTE These sequences aren't part of standard VT100 and might not be available */
void
tzn_TerminalSetCursorVisibility(U8 state)
{
  if (state == 0x01)
    fputs(SHOW_CURSOR, stdout);
  else
    fputs(HIDE_CURSOR, stdout);
}

TZN_LIKELY
void
tzn_TerminalPutChar(U8 ch)
{
  /* TODO Current implementation is suboptimal, but we need to prevent terminal from evaluating newlines and other symbols */
  fputc(ch, stdout);
  if (tzn_terminal_width == (++terminal_cursor_horizontal_pos))
  {
    terminal_cursor_horizontal_pos = 0;
    if (tzn_terminal_height == (++terminal_cursor_vertical_pos))
      terminal_cursor_vertical_pos = 0;
  }
  tzn_UpdateCursorPos();
}
