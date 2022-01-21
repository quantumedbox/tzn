#include <stdio.h>
#include <setjmp.h>

#include "tzncpu.h"
#include "tznstd.h"
#include "tznsys.h"
#include "tzndvc.h"
#include "tznerr.h"

static jmp_buf restart;
static CpuMemCB memincb; /* Memory Init Callback */

/* Registers */
static U8 regA;
static U8 regB;
static U8 regC;
static U8 regD;
static U8 regSL;
static U8 regSH;
static U8 regF; /* Flag Status Register */
static U16 regPC; /* 2 Program Counter 8-bit Registers */

static U8 ram[TZN_MTOB]; /* Lowest point of available RAM */

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
  tznLog("RAM is reserved...\n");

  /* Each sequential initialization of CPU starts from here */
  tznLog("Setting up jump...\n");
  setjmp(restart);

  tznLog("Zeroing RAM...\n");
  tznMeSet(ram, TZN_MTOB, 0x00);

  tznLog("Zeroing registers...\n");
  regA = 0x00;
  regB = 0x00;
  regC = 0x00;
  regD = 0x00;
  regSL = U16_LOW(TZN_SPIN);
  regSH = U16_HIGH(TZN_SPIN);
  regF = 0x00;
  regPC = 0x00;

  tznLog("Initializing cpu state...\n");
  tznCpuIn(ram);
  tznLog("Initializing devices...\n");
  tznDvcIn();

  while (1)
  {
#ifdef TZN_DUMP_EXEC_STATE
    fprintf(
      stdout,
      "[PC: 0x%X | A: 0x%X | B: 0x%X | C: 0x%X | D: 0x%X | L: 0x%X | H: 0x%X | S: 0x%X]\n",
      regPC,
      regA,
      regB,
      regC,
      regD,
      regSL, /* TO-CONSIDER Combine L and H into one? Same as with PC */
      regSH,
      regF
    );
#endif
    switch (ram[regPC++])
    {
      /* Register Based Instructions */

      case iMOV0A:
      {
        regA = 0;
        break;
      }
      case iMOVAB:
      {
        regB = regA;
        break;
      }
      case iMOVAC:
      {
        regC = regA;
        break;
      }
      case iMOVAD:
      {
        regD = regA;
        break;
      }
      case iMOVASL:
      {
        regSL = regA;
        break;
      }
      case iMOVASH:
      {
        regSH = regA;
        break;
      }

      /* GAP */

      case iMOVAM:
      {
        ram[U16_INIT(regB, regC)] = regA;
        break;
      }

      /* GAP */

      case iMOVBA:
      {
        regA = regB;
        break;
      }
      case iMOVCA:
      {
        regA = regC;
        break;
      }
      case iMOVDA:
      {
        regA = regD;
        break;
      }
      case iMOVSLA:
      {
        regA = regSL;
        break;
      }
      case iMOVSHA:
      {
        regA = regSH;
        break;
      }

      /* GAP */

      case iMOVMA:
      {
        regA = ram[U16_INIT(regB, regC)];
        break;
      }
      case iADDB:
      {
        regA += regB;
        regF = regA == U8_MIN;
        break;
      }
      case iSUBB:
      {
        U8 res = regA - regB;
        regF = res > regA;
        regA = res;
        break;
      }
      case iMULB:
      {
        /* TODO Status flag needs testing */
        U8 res = regA * regB;
        regF = ((regA != 0) && ((res / regA) != regB));
        regA = res;
        break;
      }
      case iDIVB:
      {
        /* TODO Handle division by zero */
        regA = regA / regB;
        break;
      }
      case iMODB:
      {
        /* TODO Handle division by zero */
        regA = regA % regB;
        break;
      }

      /* GAP */
      /* GAP */
      /* GAP */

      case iINCA:
      {
        regF = ++regA == U8_MIN;
        break;
      }
      case iDECA:
      {
        regF = --regA == U8_MAX;
        break;
      }
      case iINCB:
      {
        regF = ++regB == U8_MIN;
        break;
      }
      case iDECB:
      {
        regF = --regB == U8_MAX;
        break;
      }
      case iINCBC:
      {
        /* TODO
          Cast [B C] to U16 and use native arithmetic
          Problem with that is that Big Endian platform will require more than just casting
          One possibly solution tho is to have [B C] be ordered in ram depending on endianess of target

          For example by:
            #if TZN_ISLE == 1
              enum { rgA, rgB, rgC, rgD, rgSL, rgSH };
            #else
              enum { rgA, rgC, rgB, rgD, rgL, rgH };
            #endif
        */
        if (++regB == U8_MIN)
          regF = ++regC == U8_MIN;
        else
          regF = 0x00;
        break;
      }
      case iDECBC:
      {
        if (--regB == U8_MAX)
          regF = --regC == U8_MAX;
        else
          regF = 0x00;
        break;
      }
      case iINCM:
      {
        ++ram[U16_INIT(regB, regC)];
        break;
      }
      case iDECM:
      {
        --ram[U16_INIT(regB, regC)];
        break;
      }
      case iEQLB:
      {
        regF = regA == regB;
        break;
      }
      case iCMPB:
      {
        regF = regA < regB;
        break;
      }
      case iJMPRA:
      {
        U8 rel_addr = ram[regA];
        U16_S_I8(regPC, rel_addr);
        break;
      }
      case iJMPCRA:
      {
        U8 rel_addr = ram[regA] * regF;
        U16_S_I8(regPC, rel_addr);
        break;
      }
      case iDVCWA:
      {
        tznDvcWr(regA, regD);
        break;
      }
      case iDVCWM:
      {
        tznDvcWr(ram[U16_INIT(regB, regC)], regD);
        break;
      }
      case iDVCRA:
      {
        regA = tznDvcRd(regD);
        break;
      }
      case iDVCRM:
      {
        ram[U16_INIT(regB, regC)] = tznDvcRd(regD);
        break;
      }
      case iCALLBC:
      {
        /* Stack pointer should always point to free to rewrite spot */
        /* TODO Assert for SP underflow on debug */
        ram[U16_INIT(regSL, regSH)] = U16_LOW(regPC);
        if (--regSL == U8_MAX)
          --regSH;
        ram[U16_INIT(regSL, regSH)] = U16_HIGH(regPC);
        if (--regSL == U8_MAX)
          --regSH;
        regPC = U16_INIT(regB, regC);
        break;
      }
      case iRET:
      {
        if (++regSL == U8_MIN)
          ++regSH;
        U16_HSET(regPC, ram[U16_INIT(regSL, regSH)]);
        if (++regSL == U8_MIN)
          ++regSH;
        U16_LSET(regPC, ram[U16_INIT(regSL, regSH)]);
        break;
      }
      case iPUSHA:
      {
        ram[U16_INIT(regSL, regSH)] = regA;
        if (--regSL == U8_MAX)
          --regSH;
        break;
      }
      case iPUSHB:
      {
        ram[U16_INIT(regSL, regSH)] = regB;
        if (--regSL == U8_MAX)
          --regSH;
        break;
      }
      case iPOPA:
      {
        if (++regSL == U8_MIN)
          ++regSH;
        regA = ram[U16_INIT(regSL, regSH)];
        break;
      }
      case iPOPB:
      {
        if (++regSL == U8_MIN)
          ++regSH;
        regB = ram[U16_INIT(regSL, regSH)];
        break;
      }
      case iFLP:
      {
        regF ^= 0x01;
        break;
      }
      case iCARTOL:
      {
        regF = (regF & 0xFE) | ((regF >> 0x01) & 0x01);
        break;
      }

      /* 1 Immediate Based Instructions */

      case iMOVIA:
      {
        regA = ram[regPC++];
        break;
      }
      case iMOVIB:
      {
        regB = ram[regPC++];
        break;
      }
      case iMOVIC:
      {
        regC = ram[regPC++];
        break;
      }
      case iMOVID:
      {
        regD = ram[regPC++];
        break;
      }
      case iMOVISL:
      {
        regSL = ram[regPC++];
        break;
      }
      case iMOVISH:
      {
        regSH = ram[regPC++];
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
        regA += ram[regPC++];
        regF = regA == U8_MIN;
        break;
      }
      case iSUBI:
      {
        U8 res = regA - ram[regPC++];
        regF = res > regA;
        regA = res;
        break;
      }
      case iMULI:
      {
        /* TODO Status flag needs testing */
        U8 mul = ram[regPC++];
        U8 res = regA * mul;
        regF = ((regA != 0) && ((res / regA) != mul));
        regA = res;
        break;
      }
      case iDIVI:
      {
        /* TODO Handle division by zero */
        regA = regA / ram[regPC++];
        break;
      }
      case iMODI:
      {
        /* TODO Handle division by zero */
        regA = regA % ram[regPC++];
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
        regF = regA == ram[regPC++];
        break;
      }
      case iCMPI:
      {
        regF = regA < ram[regPC++];
        break;
      }
      case iJMPRI:
      {
        U8 rel_addr = ram[regPC++];
        U16_S_I8(regPC, rel_addr);
        break;
      }
      case iJMPCRI:
      {
        U8 rel_addr = ram[regPC++] * regF;
        U16_S_I8(regPC, rel_addr);
        break;
      }
      case iDVCWI:
      {
        tznDvcWr(ram[regPC++], regD);
        break;
      }
      /* GAP */

      /* GAP */
      /* GAP */

      case iCALLRI:
      {
        U8 rel_addr = ram[regPC++];
        ram[U16_INIT(regSL, regSH)] = U16_LOW(regPC);
        if (--regSL == U8_MAX)
          --regSH;
        ram[U16_INIT(regSL, regSH)] = U16_HIGH(regPC);
        if (--regSL == U8_MAX)
          --regSH;
        U16_S_I8(regPC, rel_addr);
        break;
      }

      /* 2 Immediate byte operating instructions */

      case iMOVIMA:
      {
        regA = ram[U16_INIT(ram[regPC], ram[regPC + 1])];
        regPC += 2;
        break;
      }
      case iMOVIAM:
      {
        ram[U16_INIT(ram[regPC], ram[regPC + 1])] = regA;
        regPC += 2;
        break;
      }
      case iDVCRIM:
      {
        ram[U16_INIT(ram[regPC], ram[regPC + 1])] = tznDvcRd(regD);
        regPC += 2;
        break;
      }
      case iCALLI:
      {
        ram[U16_INIT(regSL, regSH)] = U16_LOW(regPC);
        if (--regSL == U8_MAX)
          --regSH;
        ram[U16_INIT(regSL, regSH)] = U16_HIGH(regPC);
        if (--regSL == U8_MAX)
          --regSH;
        regPC = ram[U16_INIT(ram[regPC], ram[regPC + 1])];
        break;
      }

      default: {
        TZN_ASRT(0, "instruction not implemented");
      }
    }
  }
  TZN_DEAD();
}
