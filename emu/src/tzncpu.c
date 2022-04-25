#include "tzncpu.h"

#include <stdio.h>
#include <setjmp.h>

#include "tarch.h"
#include "tznstd.h"
#include "tznsys.h"
#include "tzndvc.h"
#include "tasrt.h"

static jmp_buf restart;
static CpuMemCB memincb; /* Memory Init Callback */

/* Registers */
static T_U8 regA;
static T_U8 regB;
static T_U8 regC;
static T_U8 regD;
static T_U8 regSL;
static T_U8 regSH;
static T_U8 regF; /* Flag Status Register */
static T_U16 regPC; /* 2 Program Counter 8-bit Registers */ /* TODO: Investigate whether `register` keyword would make gains here */

static T_U8 ram[T_PG_N * T_PG_SZ]; /* RAM mapping */

static
void
tznCpuIn(T_U8* memory)
{
  TZN_ASRT(memincb, "NULL in memory init callback");
  memincb(memory, T_PG_N * T_PG_SZ);
}

/* Used for providing ability to restart the execution */
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

T_NORET
void
tznCpuEx(void)
{
  /* Each sequential initialization of CPU starts from here */
  tznLog("Setting up jump...\n");
  setjmp(restart);

  tznLog("Zeroing RAM...\n");
  tznMeSet(ram, T_PG_N * T_PG_SZ, 0x00);

  tznLog("Zeroing registers...\n");
  regA = 0x00;
  regB = 0x00;
  regC = 0x00;
  regD = 0x00;
  regSL = T_U16_LOW(T_SPINIT);
  regSH = T_U16_HIGH(T_SPINIT);
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

      /* TODO We need alternative way of dispatching that would allow for memory savings
              And in general we need to test how our target compilers are optimizing it if at all
      */

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
        ram[T_U16_INIT(regB, regC)] = regA;
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
        regA = ram[T_U16_INIT(regB, regC)];
        break;
      }
      case iADDB:
      {
        regA += regB;
        regF = regA == T_U8_MIN;
        break;
      }
      case iSUBB:
      {
        T_U8 res = regA - regB;
        regF = res > regA;
        regA = res;
        break;
      }
      case iMULB:
      {
        /* TODO Status flag needs testing */
        T_U8 res = regA * regB;
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
        regF = ++regA == T_U8_MIN;
        break;
      }
      case iDECA:
      {
        regF = --regA == T_U8_MAX;
        break;
      }
      case iINCB:
      {
        regF = ++regB == T_U8_MIN;
        break;
      }
      case iDECB:
      {
        regF = --regB == T_U8_MAX;
        break;
      }
      case iINCBC:
      {
        if (++regB == T_U8_MIN)
          regF = ++regC == T_U8_MIN;
        else
          regF = 0x00;
        break;
      }
      case iDECBC:
      {
        if (--regB == T_U8_MAX)
          regF = --regC == T_U8_MAX;
        else
          regF = 0x00;
        break;
      }
      case iINCM:
      {
        ++ram[T_U16_INIT(regB, regC)];
        break;
      }
      case iDECM:
      {
        --ram[T_U16_INIT(regB, regC)];
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
        T_U8 rel_addr = ram[regA];
        T_U16SI8(regPC, rel_addr);
        break;
      }
      case iJMPCRA:
      {
        T_U8 rel_addr = ram[regA] * regF;
        T_U16SI8(regPC, rel_addr);
        break;
      }
      case iDVCWA:
      {
        tznDvcWr(regA, regD);
        break;
      }
      case iDVCWM:
      {
        tznDvcWr(ram[T_U16_INIT(regB, regC)], regD);
        break;
      }
      case iDVCRA:
      {
        regA = tznDvcRd(regD);
        break;
      }
      case iDVCRM:
      {
        ram[T_U16_INIT(regB, regC)] = tznDvcRd(regD);
        break;
      }
      case iCALLBC:
      {
        /* Stack pointer should always point to free to rewrite spot */
        /* TODO Assert for SP underflow on debug */
        ram[T_U16_INIT(regSL, regSH)] = T_U16_LOW(regPC);
        if (--regSL == T_U8_MAX)
          --regSH;
        ram[T_U16_INIT(regSL, regSH)] = T_U16_HIGH(regPC);
        if (--regSL == T_U8_MAX)
          --regSH;
        regPC = T_U16_INIT(regB, regC);
        break;
      }
      case iRET:
      {
        if (++regSL == T_U8_MIN)
          ++regSH;
        T_U16_HSET(regPC, ram[T_U16_INIT(regSL, regSH)]);
        if (++regSL == T_U8_MIN)
          ++regSH;
        T_U16_LSET(regPC, ram[T_U16_INIT(regSL, regSH)]);
        break;
      }
      case iPUSHA:
      {
        ram[T_U16_INIT(regSL, regSH)] = regA;
        if (--regSL == T_U8_MAX)
          --regSH;
        break;
      }
      case iPUSHB:
      {
        ram[T_U16_INIT(regSL, regSH)] = regB;
        if (--regSL == T_U8_MAX)
          --regSH;
        break;
      }
      case iPOPA:
      {
        if (++regSL == T_U8_MIN)
          ++regSH;
        regA = ram[T_U16_INIT(regSL, regSH)];
        break;
      }
      case iPOPB:
      {
        if (++regSL == T_U8_MIN)
          ++regSH;
        regB = ram[T_U16_INIT(regSL, regSH)];
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
        regF = regA == T_U8_MIN;
        break;
      }
      case iSUBI:
      {
        T_U8 res = regA - ram[regPC++];
        regF = res > regA;
        regA = res;
        break;
      }
      case iMULI:
      {
        /* TODO Status flag needs testing */
        T_U8 mul = ram[regPC++];
        T_U8 res = regA * mul;
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
        T_U8 rel_addr = ram[regPC++];
        T_U16SI8(regPC, rel_addr);
        break;
      }
      case iJMPCRI:
      {
        T_U8 rel_addr = ram[regPC++] * regF;
        T_U16SI8(regPC, rel_addr);
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
        T_U8 rel_addr = ram[regPC++];
        ram[T_U16_INIT(regSL, regSH)] = T_U16_LOW(regPC);
        if (--regSL == T_U8_MAX)
          --regSH;
        ram[T_U16_INIT(regSL, regSH)] = T_U16_HIGH(regPC);
        if (--regSL == T_U8_MAX)
          --regSH;
        T_U16SI8(regPC, rel_addr);
        break;
      }

      /* 2 Immediate byte operating instructions */

      case iMOVIMA:
      {
        regA = ram[T_U16_INIT(ram[regPC], ram[regPC + 1])];
        regPC += 2;
        break;
      }
      case iMOVIAM:
      {
        ram[T_U16_INIT(ram[regPC], ram[regPC + 1])] = regA;
        regPC += 2;
        break;
      }
      case iDVCRIM:
      {
        ram[T_U16_INIT(ram[regPC], ram[regPC + 1])] = tznDvcRd(regD);
        regPC += 2;
        break;
      }
      case iCALLI:
      {
        ram[T_U16_INIT(regSL, regSH)] = T_U16_LOW(regPC);
        if (--regSL == T_U8_MAX)
          --regSH;
        ram[T_U16_INIT(regSL, regSH)] = T_U16_HIGH(regPC);
        if (--regSL == T_U8_MAX)
          --regSH;
        regPC = ram[T_U16_INIT(ram[regPC], ram[regPC + 1])];
        break;
      }

      default: {
        TZN_ASRT(0, "Instruction not implemented");
      }
    }
  }
  TZN_DEAD();
}
