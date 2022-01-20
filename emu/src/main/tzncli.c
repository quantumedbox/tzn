#include "tzncpu.h"
#include "tznstd.h"
#include "tznsys.h"
#include "tznerr.h"

static const char* filename;

U8 def_rom[] = {
  iMOVID, 0x01,
  iMOVIB, 0x13,
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

U8 test_rom[] = {
  iMOVID, 0x03,
  iDVCWI, 0x10,
  iDVCRA,
  iEQLI, 0x00,
  iJMPCRI, -7,
  iMOVID, 0x01,
  iDVCWI, 0x13,
  iDVCWA,
  iJMPRI, -16
};

/* Sets CPU initial RAM state on startup */
static
void
defRomIn(U8* memory, U16 size)
{
  TZN_ASRT(size >= sizeof(def_rom), "Cannot fit ROM in RAM");
  tznMeCpy(memory, def_rom, sizeof(def_rom));
}

static
void
fileIn(U8* memory, U16 size)
{
  if (!filename)
    tznError("NULL filename");
  if (tznFlRd(filename, memory, size, TZN_NULL, TZN_FL0))
    tznError("Error while reading ROM");
}

int
main(int argc, char** argv)
{
  if (argc > 1)
  {
    filename = argv[1];
    tznCpuMc(fileIn);
  }
  else
    tznCpuMc(defRomIn);

  tznCpuEx();
  return 0;
}
