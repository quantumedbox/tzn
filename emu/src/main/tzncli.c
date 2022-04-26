#include "ttzn.h"
#include "tio.h"
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

/*
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
*/

/* Sets CPU initial RAM state on startup */
#ifdef T_FLSYS
  static const char* filename;

  #define T_ROM_IN(mem, sz) { \
    TZN_ASRT(((T_UPTR)(mem) + (sz)) > (T_UPTR)(mem), "RAM sz is incorrect, high address is overflowing"); \
    if (!filename) { \
      TZN_ASRT((sz) >= sizeof(def_rom), "Cannot fit ROM in RAM"); \
      tMemCopy((mem), def_rom, sizeof(def_rom)); \
    } else if (tznFlRd(filename, (mem), (sz), T_NULL, TZN_FL0)) \
      tznError("Error while reading ROM"); \
    }
#else
  #define T_ROM_IN(mem, sz) { \
    TZN_ASRT(((T_UPTR)(mem) + (sz)) > (T_UPTR)(mem), "RAM sz is incorrect, high address is overflowing"); \
    TZN_ASRT((sz) >= sizeof(def_rom), "Cannot fit ROM in RAM"); \
    tMemCopy((mem), def_rom, sizeof(def_rom)); \
  }
#endif

#include T_CPU_C

int
main(int argc, char** argv)
{
#ifdef T_FLSYS
  /* On OS hosted runtime we could receive filename as terminal argument for what ROM should be initialized on startup */
  if (argc > 1)
    filename = argv[1];
#else
  /* Otherwise internal rom is assumed */
  (void)argc;
  (void)argv;
#endif
  tznCpuEx();
  return 0;
}
