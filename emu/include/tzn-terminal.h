#ifndef TZN_TERMINAL_H
#define TZN_TERMINAL_H

#include "tzn-std.h"

#define TERMINAL_RESET                  0x00
#define TERMINAL_SET_CURSOR_HORIZONTRAL 0x10
#define TERMINAL_SET_CURSOR_VERTICAL    0x11
#define TERMINAL_SET_CURSOR_VISIBILITY  0x12
#define TERMINAL_PUT_CHAR               0x13
#define TERMINAL_PUT_STRING             0x14
#define TERMINAL_GET_DISPLAY_SIZE       0x20
#define TERMINAL_GET_CHAR               0x21

TZN_UNLIKELY void tzn_TerminalInit(void);

/*
  @brief  Write given byte to terminal device
*/
TZN_LIKELY void tzn_TerminalWrite(U8 byte);

TZN_UNLIKELY U8 tzn_TerminalRead(void);

/* Implementation dependent functions, compile against something that implements it */
extern void tzn_TerminalSetCursorHorizontalPos(U8 pos);
extern void tzn_TerminalSetCursorVerticalPos(U8 pos);
extern void tzn_TerminalSetCursorVisibility(U8 state);
TZN_LIKELY extern void tzn_TerminalPutChar(U8 ch);

extern U8 tzn_terminal_width;
extern U8 tzn_terminal_height;

#endif
