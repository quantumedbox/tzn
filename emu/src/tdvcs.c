#include "tcmplr.h"
#include "tarch.h"
#include "tio.h"
#include "tasrt.h"

/*
  TODO What about interrupt device?
  For supporting something in the lines of http://6502.org/source/kernels/minikernel.txt
  Might be quite problematic to make it portable tho
  Also, it should be possible to implement scheduler purely in TZN with available timer at this point
*/

/* Device IDs */
#define tdidCTR (T_U8)0x00U
#define tdidTRM (T_U8)0x01U
#define tdidKBD (T_U8)0x02U

#include T_CTR_D

#ifdef T_TRM_D
#include T_TRM_D
#endif

#ifdef T_KBD_D
#include T_KBD_D
#endif

static
void
tDvcInit(void)
{
#if defined(T_CTRIN)
  T_CTRIN();
#endif
#if defined(T_TRMIN)
  T_TRMIN();
#endif
#if defined(T_KBDIN)
  T_KBDIN();
#endif
}

void
tDvcFlsh(void)
{
  switch (tCpuTemp)
  {
#if defined(T_CTRFx0)
  case 0x00: { T_CTRFx0(); break; }
#endif
#if defined(T_CTRFx1)
  case 0x01: { T_CTRFx1(); break; }
#endif
#if defined(T_CTRFx2)
  case 0x02: { T_CTRFx2(); break; }
#endif
#if defined(T_CTRFx3)
  case 0x03: { T_CTRFx3(); break; }
#endif
#if defined(T_CTRFx4)
  case 0x04: { T_CTRFx4(); break; }
#endif
#if defined(T_CTRFx5)
  case 0x05: { T_CTRFx5(); break; }
#endif
#if defined(T_CTRFx6)
  case 0x06: { T_CTRFx6(); break; }
#endif
#if defined(T_CTRFx7)
  case 0x07: { T_CTRFx7(); break; }
#endif
#if defined(T_CTRFx8)
  case 0x08: { T_CTRFx8(); break; }
#endif
#if defined(T_CTRFx9)
  case 0x09: { T_CTRFx9(); break; }
#endif
#if defined(T_CTRFxA)
  case 0x0A: { T_CTRFxA(); break; }
#endif
#if defined(T_CTRFxB)
  case 0x0B: { T_CTRFxB(); break; }
#endif
#if defined(T_CTRFxC)
  case 0x0C: { T_CTRFxC(); break; }
#endif
#if defined(T_CTRFxD)
  case 0x0D: { T_CTRFxD(); break; }
#endif
#if defined(T_CTRFxE)
  case 0x0E: { T_CTRFxE(); break; }
#endif
#if defined(T_CTRFxF)
  case 0x0F: { T_CTRFxF(); break; }
#endif

#if defined(T_TRM_D)
  #if defined(T_TRMFx0)
    case 0x10: { T_TRMFx0(); break; }
  #endif
  #if defined(T_TRMFx1)
    case 0x11: { T_TRMFx1(); break; }
  #endif
  #if defined(T_TRMFx2)
    case 0x12: { T_TRMFx2(); break; }
  #endif
  #if defined(T_TRMFx3)
    case 0x13: { T_TRMFx3(); break; }
  #endif
  #if defined(T_TRMFx4)
    case 0x14: { T_TRMFx4(); break; }
  #endif
  #if defined(T_TRMFx5)
    case 0x15: { T_TRMFx5(); break; }
  #endif
  #if defined(T_TRMFx6)
    case 0x16: { T_TRMFx6(); break; }
  #endif
  #if defined(T_TRMFx7)
    case 0x17: { T_TRMFx7(); break; }
  #endif
  #if defined(T_TRMFx8)
    case 0x18: { T_TRMFx8(); break; }
  #endif
  #if defined(T_TRMFx9)
    case 0x19: { T_TRMFx9(); break; }
  #endif
  #if defined(T_TRMFxA)
    case 0x1A: { T_TRMFxA(); break; }
  #endif
  #if defined(T_TRMFxB)
    case 0x1B: { T_TRMFxB(); break; }
  #endif
  #if defined(T_TRMFxC)
    case 0x1C: { T_TRMFxC(); break; }
  #endif
  #if defined(T_TRMFxD)
    case 0x1D: { T_TRMFxD(); break; }
  #endif
  #if defined(T_TRMFxE)
    case 0x1E: { T_TRMFxE(); break; }
  #endif
  #if defined(T_TRMFxF)
    case 0x1F: { T_TRMFxF(); break; }
  #endif
#endif

  default: break;
  }
}
