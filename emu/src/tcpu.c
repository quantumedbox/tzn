/*
  TZN CPU implementation in portable C89 manner

  Requires `tRomInit` to be implemented in the same translation unit as a macro
  ```c
  #define T_ROM_IN(mem, sz)
  ```
*/

#include <stdio.h>
#include <setjmp.h>

#include "ttzn.h"
#include "tio.h"
#include "tasrt.h"

static jmp_buf tCpuRest;

/* TODO Must be zerosegment */
/* Registers */
static T_U8 tCpuRgA;
static T_U8 tCpuRgB;
static T_U8 tCpuRgC;
static T_U8 tCpuRgD;
static T_U8 tCpuRgSL;
static T_U8 tCpuRgSH;
static T_U8 tCpuRgF; /* Flag Status Register */

static T_U16 tCpuRgPC; /* 2 Program Counter 8-bit Registers */ /* TODO: Investigate whether `register` keyword would make gains here */

static T_U8 tCpuRam[T_PG_N * T_PG_SZ]; /* RAM mapping */

#define tCpuRes_() longjmp(tCpuRest, 0)

static T_U8 tCpuDvIn; /* Device intermediate, used for bypassing stack in device communication */
#include T_DVCS_C

T_NORET
void
tznCpuEx(void)
{
  /* Each sequential initialization of CPU starts from here */
  tznLog("Setting up jump...\n");
  setjmp(tCpuRest);

  tznLog("Zeroing RAM...\n");
  tMemSet(tCpuRam, 0x00, T_PG_N * T_PG_SZ);

  tznLog("Zeroing registers...\n");
  tCpuRgA = 0x00;
  tCpuRgB = 0x00;
  tCpuRgC = 0x00;
  tCpuRgD = 0x00;
  tCpuRgSL = T_U16_LOW(T_SPINIT);
  tCpuRgSH = T_U16_HIGH(T_SPINIT);
  tCpuRgF = 0x00;
  tCpuRgPC = 0x00;

  tznLog("Initializing cpu state...\n");
  T_ROM_IN(tCpuRam, T_PG_N * T_PG_SZ);

  tznLog("Initializing devices...\n");
  tznDvcIn();

  while (1)
  {
#ifdef TZN_DUMP_EXEC_STATE
    fprintf(
      stdout,
      "[PC: 0x%X | A: 0x%X | B: 0x%X | C: 0x%X | D: 0x%X | L: 0x%X | H: 0x%X | S: 0x%X]\n",
      tCpuRgPC,
      tCpuRgA,
      tCpuRgB,
      tCpuRgC,
      tCpuRgD,
      tCpuRgSL, /* TODO Combine L and H into one? Same as with PC, building U16 values all the time might be costly */
      tCpuRgSH,
      tCpuRgF
    );
#endif
    switch (tCpuRam[tCpuRgPC++])
    {
      /* Register Based Instructions */

      /* TODO We need alternative way of dispatching that would allow for memory savings
              And in general we need to test how our target compilers are optimizing it if at all

         TODO CC65 Produces individual comparison for each and every branch which is unacceptable
              We need to have jump table, in unique implementation
      */

      case iMOV0A:
      {
        tCpuRgA = 0;
        break;
      }
      case iMOVAB:
      {
        tCpuRgB = tCpuRgA;
        break;
      }
      case iMOVAC:
      {
        tCpuRgC = tCpuRgA;
        break;
      }
      case iMOVAD:
      {
        tCpuRgD = tCpuRgA;
        break;
      }
      case iMOVASL:
      {
        tCpuRgSL = tCpuRgA;
        break;
      }
      case iMOVASH:
      {
        tCpuRgSH = tCpuRgA;
        break;
      }

      /* GAP */

      case iMOVAM:
      {
        tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)] = tCpuRgA;
        break;
      }

      /* GAP */

      case iMOVBA:
      {
        tCpuRgA = tCpuRgB;
        break;
      }
      case iMOVCA:
      {
        tCpuRgA = tCpuRgC;
        break;
      }
      case iMOVDA:
      {
        tCpuRgA = tCpuRgD;
        break;
      }
      case iMOVSLA:
      {
        tCpuRgA = tCpuRgSL;
        break;
      }
      case iMOVSHA:
      {
        tCpuRgA = tCpuRgSH;
        break;
      }

      /* GAP */

      case iMOVMA:
      {
        tCpuRgA = tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)];
        break;
      }
      case iADDB:
      {
        tCpuRgA += tCpuRgB;
        tCpuRgF = tCpuRgA == T_U8_MIN;
        break;
      }
      case iSUBB:
      {
        T_U8 res = tCpuRgA - tCpuRgB;
        tCpuRgF = res > tCpuRgA;
        tCpuRgA = res;
        break;
      }
      case iMULB:
      {
        /* TODO Status flag needs testing */
        T_U8 res = tCpuRgA * tCpuRgB;
        tCpuRgF = ((tCpuRgA != 0) && ((res / tCpuRgA) != tCpuRgB));
        tCpuRgA = res;
        break;
      }
      case iDIVB:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA / tCpuRgB;
        break;
      }
      case iMODB:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA % tCpuRgB;
        break;
      }

      /* GAP */
      /* GAP */
      /* GAP */

      case iINCA:
      {
        tCpuRgF = ++tCpuRgA == T_U8_MIN;
        break;
      }
      case iDECA:
      {
        tCpuRgF = --tCpuRgA == T_U8_MAX;
        break;
      }
      case iINCB:
      {
        tCpuRgF = ++tCpuRgB == T_U8_MIN;
        break;
      }
      case iDECB:
      {
        tCpuRgF = --tCpuRgB == T_U8_MAX;
        break;
      }
      case iINCBC:
      {
        if (++tCpuRgB == T_U8_MIN)
          tCpuRgF = ++tCpuRgC == T_U8_MIN;
        else
          tCpuRgF = 0x00;
        break;
      }
      case iDECBC:
      {
        if (--tCpuRgB == T_U8_MAX)
          tCpuRgF = --tCpuRgC == T_U8_MAX;
        else
          tCpuRgF = 0x00;
        break;
      }
      case iINCM:
      {
        ++tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)];
        break;
      }
      case iDECM:
      {
        --tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)];
        break;
      }
      case iEQLB:
      {
        tCpuRgF = tCpuRgA == tCpuRgB;
        break;
      }
      case iCMPB:
      {
        tCpuRgF = tCpuRgA < tCpuRgB;
        break;
      }
      case iJMPRA:
      {
        T_U8 rel_addr = tCpuRam[tCpuRgA];
        T_U16SI8(tCpuRgPC, rel_addr);
        break;
      }
      case iJMPCRA:
      {
        T_U8 rel_addr = tCpuRam[tCpuRgA] * tCpuRgF;
        T_U16SI8(tCpuRgPC, rel_addr);
        break;
      }
      case iDVCWA:
      {
        tCpuDvIn = tCpuRgA;
        tznDvcWr();
        break;
      }
      case iDVCWM:
      {
        tCpuDvIn = tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)];
        tznDvcWr();
        break;
      }
      case iDVCRA:
      {
        tznDvcRd();
        tCpuRgA = tCpuDvIn;
        break;
      }
      case iDVCRM:
      {
        tznDvcRd();
        tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)] = tCpuDvIn;
        break;
      }
      case iCALLBC:
      {
        /* Stack pointer should always point to free to rewrite spot */
        /* TODO Assert for SP underflow on debug */
        tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)] = T_U16_LOW(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)] = T_U16_HIGH(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        tCpuRgPC = T_U16_INIT(tCpuRgB, tCpuRgC);
        break;
      }
      case iRET:
      {
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        T_U16_HSET(tCpuRgPC, tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)]);
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        T_U16_LSET(tCpuRgPC, tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)]);
        break;
      }
      case iPUSHA:
      {
        tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)] = tCpuRgA;
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        break;
      }
      case iPUSHB:
      {
        tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)] = tCpuRgB;
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        break;
      }
      case iPOPA:
      {
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        tCpuRgA = tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)];
        break;
      }
      case iPOPB:
      {
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        tCpuRgB = tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)];
        break;
      }
      case iFLP:
      {
        tCpuRgF ^= 0x01;
        break;
      }
      case iCARTOL:
      {
        tCpuRgF = (tCpuRgF & 0xFE) | ((tCpuRgF >> 0x01) & 0x01);
        break;
      }

      /* 1 Immediate Based Instructions */

      case iMOVIA:
      {
        tCpuRgA = tCpuRam[tCpuRgPC++];
        break;
      }
      case iMOVIB:
      {
        tCpuRgB = tCpuRam[tCpuRgPC++];
        break;
      }
      case iMOVIC:
      {
        tCpuRgC = tCpuRam[tCpuRgPC++];
        break;
      }
      case iMOVID:
      {
        tCpuRgD = tCpuRam[tCpuRgPC++];
        break;
      }
      case iMOVISL:
      {
        tCpuRgSL = tCpuRam[tCpuRgPC++];
        break;
      }
      case iMOVISH:
      {
        tCpuRgSH = tCpuRam[tCpuRgPC++];
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
        tCpuRgA += tCpuRam[tCpuRgPC++];
        tCpuRgF = tCpuRgA == T_U8_MIN;
        break;
      }
      case iSUBI:
      {
        T_U8 res = tCpuRgA - tCpuRam[tCpuRgPC++];
        tCpuRgF = res > tCpuRgA;
        tCpuRgA = res;
        break;
      }
      case iMULI:
      {
        /* TODO Status flag needs testing */
        T_U8 mul = tCpuRam[tCpuRgPC++];
        T_U8 res = tCpuRgA * mul;
        tCpuRgF = ((tCpuRgA != 0) && ((res / tCpuRgA) != mul));
        tCpuRgA = res;
        break;
      }
      case iDIVI:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA / tCpuRam[tCpuRgPC++];
        break;
      }
      case iMODI:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA % tCpuRam[tCpuRgPC++];
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
        tCpuRgF = tCpuRgA == tCpuRam[tCpuRgPC++];
        break;
      }
      case iCMPI:
      {
        tCpuRgF = tCpuRgA < tCpuRam[tCpuRgPC++];
        break;
      }
      case iJMPRI:
      {
        T_U8 rel_addr = tCpuRam[tCpuRgPC++];
        T_U16SI8(tCpuRgPC, rel_addr);
        break;
      }
      case iJMPCRI:
      {
        T_U8 rel_addr = tCpuRam[tCpuRgPC++] * tCpuRgF;
        T_U16SI8(tCpuRgPC, rel_addr);
        break;
      }
      case iDVCWI:
      {
        tCpuDvIn = tCpuRam[tCpuRgPC++];
        tznDvcWr();
        break;
      }
      /* GAP */

      /* GAP */
      /* GAP */

      case iCALLRI:
      {
        T_U8 rel_addr = tCpuRam[tCpuRgPC++];
        tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)] = T_U16_LOW(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)] = T_U16_HIGH(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        T_U16SI8(tCpuRgPC, rel_addr);
        break;
      }

      /* 2 Immediate byte operating instructions */

      case iMOVIMA:
      {
        tCpuRgA = tCpuRam[T_U16_INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])];
        tCpuRgPC += 2;
        break;
      }
      case iMOVIAM:
      {
        tCpuRam[T_U16_INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])] = tCpuRgA;
        tCpuRgPC += 2;
        break;
      }
      case iDVCRIM:
      {
        tznDvcRd();
        tCpuRam[T_U16_INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])] = tCpuDvIn;
        tCpuRgPC += 2;
        break;
      }
      case iCALLI:
      {
        tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)] = T_U16_LOW(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)] = T_U16_HIGH(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        tCpuRgPC = tCpuRam[T_U16_INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])];
        break;
      }

      default: {
        TZN_ASRT(0, "Instruction not implemented");
      }
    }
  }
  TZN_DEAD();
}
