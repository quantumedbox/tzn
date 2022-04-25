#include "tarch.h"
#include "tzncpu.h"
#include "tznstd.h"
#include "tznsys.h"
#include "tasrt.h"

/* TODO: Implement mechanism for embedding custom ROMs */

T_U8 def_rom[] = {
  iMOVID, 0x01,
  iMOVIB, 0x13,
  iDVCWI, 0x14,
  iDVCWM,
  iMOVMA,
  iEQLI, 0x00,
  iJMPCRI, 3,
  iINCBC,
  iJMPRI, -9,
  iMOVID, 0x00,
  iDVCWI, 0x02,
  'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0x00
};

T_U8 test_rom[] = {
  iMOVID, 0x01,
  iDVCWI, 0x10,
  iDVCRA,
  iEQLI, 0x00,
  iJMPCRI, -7,
  iMOVID, 0x00,
  iDVCWI, 0x13,
  iDVCWA,
  iJMPRI, -16
};

/* Sets CPU initial RAM state on startup */
static
void
defRomIn(T_U8* memory, T_U16 size)
{
  TZN_ASRT(((T_UPTR)memory + size) > (T_UPTR)memory, "RAM size is incorrect, high address is overflowing");
  TZN_ASRT(size >= sizeof(def_rom), "Cannot fit ROM in RAM");
  tznMeCpy(memory, def_rom, sizeof(def_rom));
}

#ifdef T_FLSYS
  static const char* filename;

  static
  void
  fileIn(T_U8* memory, T_U16 size)
  {
    if (!filename)
      tznError("NULL filename");
    if (tznFlRd(filename, memory, size, T_NULL, TZN_FL0))
      tznError("Error while reading ROM");
  }
#endif

int
main(int argc, char** argv)
{
#ifdef T_FLSYS
  /* On OS hosted runtime we could receive filename as terminal argument for what ROM should be initialized on startup */
  if (argc > 1)
  {
    filename = argv[1];
    tznCpuMc(fileIn);
  }
  else
    tznCpuMc(defRomIn);
#else
  /* Otherwise internal rom is assumed */
  (void)argc;
  (void)argv;
  tznCpuMc(defRomIn);
#endif
  tznCpuEx();
  return 0;
}
