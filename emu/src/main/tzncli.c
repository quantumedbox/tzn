#include "tzncpu.h"
#include "tznstd.h"

#include "assert.h"

/*U8 hello_world_rom[] = {
  iMOVID, 0x01,
  iMOVIB, 0x12,
  iMOVIC, 0x01,
  DVWI, 0x14,
  DVWM,
  iMOVMA,
  EQLI, 0x00,
  JMPCRI, 3,
  iINCBC,
  JMPRI, -10,
  JMPRI, -2,
  'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0x00
};*/

U8 test[] = {
  iMOVID, 0x01,
  iMOVIB, 0x14,
  iMOVIC, 0x01,
  iDVCWI, 0x14,
  iDVCWM,
  iMOVMA,
  iEQLI, 0x00,
  iJMPCRI, 3,
  iINCBC,
  iJMPRI, -9,
  iMOVID, 0x02,
  iDVCWI, 0x01,
  'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0x00
};

/* Sets CPU initial RAM state on startup */
static
void memInit(U8* memory, U16 size)
{
  U16 idx = sizeof(test);
  TZN_ASRT(size >= sizeof(test), "Cannot fit ROM in RAM");
  while (idx--)
    memory[idx] = test[idx];
}

int
main(void)
{
  tznCpuMc(memInit);
  tznCpuEx();

  return 0;
}
