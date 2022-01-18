#include "tzncpu.h"
#include "tznstd.h"

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

/* Sets CPU initial RAM state on startup */
static
void memoryInit(U8* memory, U16 size)
{
  U16 idx = sizeof(test);
  TZN_ASSERT(size >= sizeof(test), "Cannot fit ROM in RAM");
  while (idx--)
    memory[idx] = test[idx];
}

int
main(void)
{
  tzn_CpuRegisterMemoryInitCallback(memoryInit);
  tzn_CpuExec();

  return 0;
}
