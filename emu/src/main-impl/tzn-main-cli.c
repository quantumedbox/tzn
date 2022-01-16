#include "tzn-cpu.h"
#include "tzn-std.h"

U8 hello_world_rom[] = {
  SETD, 0x01,
  SETA, 0x14,
  SETB, 0x01,
  DVWI, 0x14,
  DVWM,
  MOVAC,
  MOVMA,
  EQLI, 0x00,
  JMPCRI, 4,
  MOVCA,
  INCAB,
  JMPRI, -11,
  JMPRI, -2, /* Halt in loop */
  'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0x00
};

int
main(void)
{
  tzn_WriteMemory(hello_world_rom, TZN_MEMORY_RAM_START, sizeof hello_world_rom);
  tzn_Exec();

  return 0;
}
