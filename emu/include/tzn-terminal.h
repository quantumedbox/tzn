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

void tzn_TerminalInit(void);
TZN_LIKELY void tzn_TerminalWrite(U8 byte);
U8 tzn_TerminalRead(void);

#endif
