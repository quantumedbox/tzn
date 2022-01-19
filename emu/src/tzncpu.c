#include <stdio.h>
#include <setjmp.h>

#include "tzncpu.h"
#include "tznstd.h"
#include "tznsys.h"
#include "tzndvc.h"
#include "tznerr.h"

static jmp_buf restart;
static CpuMemCB memincb; /* Memory Init Callback */

enum { rgA, rgB, rgC, rgD, rgL, rgH };

static
void
tznCpuIn(U8* memory)
{
  TZN_ASRT(memincb, "NULL in memory init callback");
  memincb(&memory[TZN_MRMS], TZN_MRMB);
}

void
tznCpuRs(void)
{
  longjmp(restart, 0);
}

void
tznCpuMc(CpuMemCB callback)
{
  TZN_ASRT(callback, "NULL passed in tznCpuMc()");
  memincb = callback;
}

TZN_NORE
void
tznCpuEx(void)
{
  /* TODO Only provide RAM? Not full address range */
  /*
    TODO Combine related field into handler to pass around,
         for example, that way we could have INIT_REGISTER_STATE as function instead of macro
  */
  U8 memory[TZN_MTOB];
  register U16 pgc_r; /* Program Counter Registers */
  U8 regs[TZN_REGG]; /* Registers */
  U8 status_r; /* Status Register */

  setjmp(restart);

  tznCpuIn(memory);
  tznDvcIn();

#define INIT_REGISTER_STATE() { \
    U16 idx = TZN_REGG; \
    while (idx--) \
      regs[idx] = 0x00; \
    pgc_r = TZN_MRMS; \
    status_r = 0x00; \
    regs[rgL] = U16_LOW(TZN_SPIN); \
    regs[rgH] = U16_HIGH(TZN_SPIN); \
  }

  INIT_REGISTER_STATE();

  while (1)
  {
#ifdef TZN_DUMP_EXEC_STATE
    fprintf(
      stdout,
      "[PC: 0x%X | A: 0x%X | B: 0x%X | C: 0x%X | D: 0x%X | L: 0x%X | H: 0x%X | S: 0x%X]\n",
      pgc_r,
      regs[rgA],
      regs[rgB],
      regs[rgC],
      regs[rgD],
      regs[rgL], /* TO-CONSIDER Combine L and H into one? Same as with PC */
      regs[rgH],
      status_r
    );
#endif
    switch (memory[pgc_r++])
    {
      case iMOV0A:
      {
        regs[rgA] = 0;
        break;
      }
      case iMOVIA:
      {
        regs[rgA] = memory[pgc_r++];
        break;
      }
      case iMOVIB:
      {
        regs[rgB] = memory[pgc_r++];
        break;
      }
      case iMOVIC:
      {
        regs[rgC] = memory[pgc_r++];
        break;
      }
      case iMOVID:
      {
        regs[rgD] = memory[pgc_r++];
        break;
      }
      case iMOVAB:
      {
        regs[rgB] = regs[rgA];
        break;
      }
      case iMOVAC:
      {
        regs[rgC] = regs[rgA];
        break;
      }
      case iMOVAD:
      {
        regs[rgD] = regs[rgA];
        break;
      }
      case iMOVBA:
      {
        regs[rgA] = regs[rgB];
        break;
      }
      case iMOVCA:
      {
        regs[rgA] = regs[rgC];
        break;
      }
      case iMOVDA:
      {
        regs[rgA] = regs[rgD];
        break;
      }
      case iMOVMA:
      {
        regs[rgA] = memory[U16_INIT(regs[rgB], regs[rgC])];
        break;
      }
      case iINCA:
      {
        regs[rgA]++;
        status_r = regs[rgA] == U8_MIN;
        break;
      }
      case iDECA:
      {
        regs[rgA]--;
        status_r = regs[rgA] == U8_MAX;
        break;
      }
      case iINCBC:
      {
        /* TODO
          Cast [B C] to U16 and use native arithmetic
          Problem with that is that Big Endian platform will require more than just casting
          One possibly solution tho is to have [B C] be ordered in memory depending on endianess of target

          For example by:
            #if TZN_ISLE == 1
              enum { rgA, rgB, rgC, rgD, rgSL, rgSH };
            #else
              enum { rgA, rgC, rgB, rgD, rgL, rgH };
            #endif
        */
        if (++regs[rgB] == U8_MIN)
          status_r = ++regs[rgC] == U8_MIN;
        else
          status_r = 0x00;
        break;
      }
      case iDECBC:
      {
        if (--regs[rgB] == U8_MAX)
          status_r = --regs[rgC] == U8_MAX;
        else
          status_r = 0x00;
        break;
      }
      case iFLP:
      {
        status_r ^= 0x01;
        break;
      }
      case iADDI:
      {
        regs[rgA] += memory[pgc_r++];
        status_r = regs[rgA] == U8_MIN;
        break;
      }
      case iADDB:
      {
        regs[rgA] += regs[rgB];
        status_r = regs[rgA] == U8_MIN;
        break;
      }
      case iSUBI:
      {
        U8 res = regs[rgA] - memory[pgc_r++];
        status_r = res > regs[rgA];
        regs[rgA] = res;
        break;
      }
      case iSUBB:
      {
        U8 res = regs[rgA] - regs[rgB];
        status_r = res > regs[rgA];
        regs[rgA] = res;
        break;
      }
      case iEQLI:
      {
        status_r = regs[rgA] == memory[pgc_r++];
        break;
      }
      case iEQLB:
      {
        status_r = regs[rgA] == regs[rgB];
        break;
      }
      case iJMPRI:
      {
        U8 rel_addr = memory[pgc_r++];
        U16_S_I8(pgc_r, rel_addr);
        break;
      }
      case iJMPCRI:
      {
        U8 rel_addr = memory[pgc_r++] * status_r;
        U16_S_I8(pgc_r, rel_addr);
        break;
      }
      case iDVCWI:
      {
        tznDvcWr(memory[pgc_r++], regs[rgD]);
        break;
      }
      case iDVCWA:
      {
        tznDvcWr(regs[rgA], regs[rgD]);
        break;
      }
      case iDVCWM:
      {
        tznDvcWr(memory[U16_INIT(regs[rgB], regs[rgC])], regs[rgD]);
        break;
      }
      case iDVCRA:
      {
        regs[rgA] = tznDvcRd(regs[rgD]);
        break;
      }
      default: {
        TZN_ASRT(0, "instruction not implemented");
      }
    }
  }
#undef INIT_REGISTER_STATE
}
