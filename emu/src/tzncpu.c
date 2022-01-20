#include <stdio.h>
#include <setjmp.h>

#include "tzncpu.h"
#include "tznstd.h"
#include "tznsys.h"
#include "tzndvc.h"
#include "tznerr.h"

static jmp_buf restart;
static CpuMemCB memincb; /* Memory Init Callback */

enum { rgA, rgB, rgC, rgD, rgSL, rgSH };

static
void
tznCpuIn(U8* memory)
{
  TZN_ASRT(memincb, "NULL in memory init callback");
  memincb(memory, TZN_MTOB);
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

  tznMeSet(memory, TZN_MTOB, 0x00);
  tznMeSet(regs, sizeof(regs[0]) * TZN_REGG, 0x00);
  pgc_r = 0x00;
  status_r = 0x00;
  regs[rgSL] = U16_LOW(TZN_SPIN);
  regs[rgSH] = U16_HIGH(TZN_SPIN);

  tznCpuIn(memory);
  tznDvcIn();

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
      /* Register Based Instructions */

      case iMOV0A:
      {
        regs[rgA] = 0;
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
      case iMOVASL:
      {
        regs[rgSL] = regs[rgA];
        break;
      }
      case iMOVASH:
      {
        regs[rgSH] = regs[rgA];
        break;
      }

      /* GAP */

      case iMOVAM:
      {
        memory[U16_INIT(regs[rgB], regs[rgC])] = regs[rgA];
        break;
      }

      /* GAP */

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
      case iMOVSLA:
      {
        regs[rgA] = regs[rgSL];
        break;
      }
      case iMOVSHA:
      {
        regs[rgA] = regs[rgSH];
        break;
      }

      /* GAP */

      case iMOVMA:
      {
        regs[rgA] = memory[U16_INIT(regs[rgB], regs[rgC])];
        break;
      }
      case iADDB:
      {
        regs[rgA] += regs[rgB];
        status_r = regs[rgA] == U8_MIN;
        break;
      }
      case iSUBB:
      {
        U8 res = regs[rgA] - regs[rgB];
        status_r = res > regs[rgA];
        regs[rgA] = res;
        break;
      }
      case iMULB:
      {
        /* TODO Status flag needs testing */
        U8 res = regs[rgA] * regs[rgB];
        status_r = ((regs[rgA] != 0) && ((res / regs[rgA]) != regs[rgB]));
        regs[rgA] = res;
        break;
      }
      case iDIVB:
      {
        /* TODO Handle division by zero */
        regs[rgA] = regs[rgA] / regs[rgB];
        break;
      }
      case iMODB:
      {
        /* TODO Handle division by zero */
        regs[rgA] = regs[rgA] % regs[rgB];
        break;
      }

      /* GAP */
      /* GAP */
      /* GAP */

      case iINCA:
      {
        status_r = ++regs[rgA] == U8_MIN;
        break;
      }
      case iDECA:
      {
        status_r = --regs[rgA] == U8_MAX;
        break;
      }
      case iINCB:
      {
        status_r = ++regs[rgB] == U8_MIN;
        break;
      }
      case iDECB:
      {
        status_r = --regs[rgB] == U8_MAX;
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
      case iINCM:
      {
        ++memory[U16_INIT(regs[rgB], regs[rgC])];
        break;
      }
      case iDECM:
      {
        --memory[U16_INIT(regs[rgB], regs[rgC])];
        break;
      }
      case iEQLB:
      {
        status_r = regs[rgA] == regs[rgB];
        break;
      }
      case iCMPB:
      {
        status_r = regs[rgA] < regs[rgB];
        break;
      }
      case iJMPRA:
      {
        U8 rel_addr = memory[regs[rgA]];
        U16_S_I8(pgc_r, rel_addr);
        break;
      }
      case iJMPCRA:
      {
        U8 rel_addr = memory[regs[rgA]] * status_r;
        U16_S_I8(pgc_r, rel_addr);
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
      case iDVCRM:
      {
        memory[U16_INIT(regs[rgB], regs[rgC])] = tznDvcRd(regs[rgD]);
        break;
      }
      case iCALLBC:
      {
        /* Stack pointer should always point to free to rewrite spot */
        /* TODO Assert for SP underflow on debug */
        memory[U16_INIT(regs[rgSL], regs[rgSH])] = U16_LOW(pgc_r);
        if (--regs[rgSL] == U8_MAX)
          --regs[rgSH];
        memory[U16_INIT(regs[rgSL], regs[rgSH])] = U16_HIGH(pgc_r);
        if (--regs[rgSL] == U8_MAX)
          --regs[rgSH];
        pgc_r = U16_INIT(regs[rgB], regs[rgC]);
        break;
      }
      case iRET:
      {
        if (++regs[rgSL] == U8_MIN)
          ++regs[rgSH];
        U16_HSET(pgc_r, memory[U16_INIT(regs[rgSL], regs[rgSH])]);
        if (++regs[rgSL] == U8_MIN)
          ++regs[rgSH];
        U16_LSET(pgc_r, memory[U16_INIT(regs[rgSL], regs[rgSH])]);
        break;
      }
      case iPUSHA:
      {
        memory[U16_INIT(regs[rgSL], regs[rgSH])] = regs[rgA];
        if (--regs[rgSL] == U8_MAX)
          --regs[rgSH];
        break;
      }
      case iPUSHB:
      {
        memory[U16_INIT(regs[rgSL], regs[rgSH])] = regs[rgB];
        if (--regs[rgSL] == U8_MAX)
          --regs[rgSH];
        break;
      }
      case iPOPA:
      {
        if (++regs[rgSL] == U8_MIN)
          ++regs[rgSH];
        regs[rgA] = memory[U16_INIT(regs[rgSL], regs[rgSH])];
        break;
      }
      case iPOPB:
      {
        if (++regs[rgSL] == U8_MIN)
          ++regs[rgSH];
        regs[rgB] = memory[U16_INIT(regs[rgSL], regs[rgSH])];
        break;
      }
      case iFLP:
      {
        status_r ^= 0x01;
        break;
      }
      case iCARTOL:
      {
        status_r = (status_r & 0xFE) | ((status_r >> 0x01) & 0x01);
        break;
      }

      /* 1 Immediate Based Instructions */

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
      case iMOVISL:
      {
        regs[rgSL] = memory[pgc_r++];
        break;
      }
      case iMOVISH:
      {
        regs[rgSH] = memory[pgc_r++];
        break;
      }

      /* GAP */
      /* GAP */

      /* GAP */
      /* GAP */
      /* GAP */
      /* GAP */
      /* GAP */
      /* GAP */
      /* GAP */
      /* GAP */

      case iADDI:
      {
        regs[rgA] += memory[pgc_r++];
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
      case iMULI:
      {
        /* TODO Status flag needs testing */
        U8 mul = memory[pgc_r++];
        U8 res = regs[rgA] * mul;
        status_r = ((regs[rgA] != 0) && ((res / regs[rgA]) != mul));
        regs[rgA] = res;
        break;
      }
      case iDIVI:
      {
        /* TODO Handle division by zero */
        regs[rgA] = regs[rgA] / memory[pgc_r++];
        break;
      }
      case iMODI:
      {
        /* TODO Handle division by zero */
        regs[rgA] = regs[rgA] % memory[pgc_r++];
        break;
      }

      /* GAP */
      /* GAP */
      /* GAP */

      /* GAP */
      /* GAP */
      /* GAP */
      /* GAP */
      /* GAP */
      /* GAP */
      /* GAP */
      /* GAP */

      case iEQLI:
      {
        status_r = regs[rgA] == memory[pgc_r++];
        break;
      }
      case iCMPI:
      {
        status_r = regs[rgA] < memory[pgc_r++];
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
      /* GAP */

      /* GAP */
      /* GAP */

      case iCALLRI:
      {
        U8 rel_addr = memory[pgc_r++];
        memory[U16_INIT(regs[rgSL], regs[rgSH])] = U16_LOW(pgc_r);
        if (--regs[rgSL] == U8_MAX)
          --regs[rgSH];
        memory[U16_INIT(regs[rgSL], regs[rgSH])] = U16_HIGH(pgc_r);
        if (--regs[rgSL] == U8_MAX)
          --regs[rgSH];
        U16_S_I8(pgc_r, rel_addr);
        break;
      }

      /* 2 Immediate byte operating instructions */

      case iMOVIMA:
      {
        regs[rgA] = memory[U16_INIT(memory[pgc_r], memory[pgc_r + 1])];
        pgc_r += 2;
        break;
      }
      case iMOVIAM:
      {
        memory[U16_INIT(memory[pgc_r], memory[pgc_r + 1])] = regs[rgA];
        pgc_r += 2;
        break;
      }
      case iDVCRIM:
      {
        memory[U16_INIT(memory[pgc_r], memory[pgc_r + 1])] = tznDvcRd(regs[rgD]);
        pgc_r += 2;
        break;
      }
      case iCALLI:
      {
        memory[U16_INIT(regs[rgSL], regs[rgSH])] = U16_LOW(pgc_r);
        if (--regs[rgSL] == U8_MAX)
          --regs[rgSH];
        memory[U16_INIT(regs[rgSL], regs[rgSH])] = U16_HIGH(pgc_r);
        if (--regs[rgSL] == U8_MAX)
          --regs[rgSH];
        pgc_r = memory[U16_INIT(memory[pgc_r], memory[pgc_r + 1])];
        break;
      }

      default: {
        TZN_ASRT(0, "instruction not implemented");
      }
    }
  }
  TZN_DEAD();
}
