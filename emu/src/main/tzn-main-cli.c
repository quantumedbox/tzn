#include "tzn-cpu.h"
#include "tzn-std.h"

U8 hello_world_rom[] = {
  SETD, 0x01,
  SETB, 0x12,
  SETC, 0x01,
  DVWI, 0x14,
  DVWM,
  MOVMA,
  EQLI, 0x00,
  JMPCRI, 3,
  INCBC,
  JMPRI, -10,
  JMPRI, -2, /* Halt in loop */
  'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0x00
};

U8 test[] = {
  SETD, 0x01,
  SETB, 0x14,
  SETC, 0x01,
  DVWI, 0x14,
  DVWM,
  MOVMA,
  EQLI, 0x00,
  JMPCRI, 3,
  INCBC,
  JMPRI, -10,
  SETD, 0x02,
  DVWI, 0x02,
  'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0x00
};

int
main(void)
{
  tzn_CpuPassInitMemory(test, sizeof test);
  tzn_CpuExec();

  return 0;
}
