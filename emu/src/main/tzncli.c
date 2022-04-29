#include "ttzn.h"
#include "tio.h"
#include "tasrt.h"

/* TODO: Implement mechanism for embedding custom ROMs */

static const T_U8 def_rom[] = {
  tiMOVIB, 0x0F,
  tiOUTIM, 0x16,
  tiMOVMA,
  tiEQLI, 0x00,
  tiJMPCRI, -9,
  tiINCBC,
  tiJMPRI, -10,
  'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', ' ', 0x00
};

/* Sets CPU initial RAM state on startup */
#ifdef T_IO_C
  static const char* filename;

  #define T_ROM_IN(mem, sz) { \
    TZN_ASRT(((T_UPTR)(mem) + (sz)) > (T_UPTR)(mem), "RAM sz is incorrect, high address is overflowing"); \
    if (!filename) { \
      TZN_ASRT((sz) >= sizeof(def_rom), "Cannot fit ROM in RAM"); \
      tMemCopy((mem), def_rom, sizeof(def_rom)); \
    } else if (tznFlRd(filename, (mem), (sz), T_NULL, TZN_FL0)) \
      tError("Error while reading ROM"); \
    }
#else

  #define T_ROM_IN(mem, sz) { \
    TZN_ASRT(((T_UPTR)(mem) + (sz)) > (T_UPTR)(mem), "RAM sz is incorrect, high address is overflowing"); \
    TZN_ASRT((sz) >= sizeof(def_rom), "Cannot fit ROM in RAM"); \
    tMemCopy((mem), def_rom, sizeof(def_rom)); \
  }
#endif

#include T_CPU_C

#if defined(T_CMDARG) && defined(T_IO_C)
    int
    main(int argc, char** argv)
    {
      if (argc > 1)
        filename = argv[1];

      tCpuExec();
      return 0;
    }

#else
  int
  main()
  {
    tCpuExec();
    return 0;
  }
#endif
