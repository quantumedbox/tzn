/*
  VT100 based terminal implementation
*/

#include <stdio.h>

#include "tzn-std.h"

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 25

#define ESCAPE_SEQUENCE "\e["
#define HIDE_CURSOR ESCAPE_SEQUENCE "?25l"
#define SHOW_CURSOR ESCAPE_SEQUENCE "?25h"
#define CLEAR_SCREEN ESCAPE_SEQUENCE "2J"

const U8 tzn_terminal_width = TERMINAL_WIDTH;
const U8 tzn_terminal_height = TERMINAL_HEIGHT;

static U8 terminal_cursor_horizontal_pos;
static U8 terminal_cursor_vertical_pos;
/* static U8 terminal_cursor_visibility; */

static U8 screen_map[TERMINAL_WIDTH * TERMINAL_HEIGHT];

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

void
tzn_TerminalInitImpl(void)
{
  terminal_cursor_horizontal_pos = 0;
  terminal_cursor_vertical_pos = 0;
  tzn_UpdateCursorPos();

  fputs(CLEAR_SCREEN, stdout);

  U16 idx = TERMINAL_WIDTH * TERMINAL_HEIGHT;
  while (idx--)
    screen_map[idx] = 0;
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
  screen_map[terminal_cursor_horizontal_pos + terminal_cursor_vertical_pos * tzn_terminal_width] = ch;
  fputc(ch, stdout);
  if (tzn_terminal_width == (++terminal_cursor_horizontal_pos))
  {
    terminal_cursor_horizontal_pos = 0;
    if (tzn_terminal_height == (++terminal_cursor_vertical_pos))
      terminal_cursor_vertical_pos = 0;
  }
  tzn_UpdateCursorPos();
}

U8
tzn_TerminalGetChar(U8 horizontal, U8 vertical)
{
  return screen_map[horizontal + vertical * tzn_terminal_width];
}
