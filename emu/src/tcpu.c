/*
  TZN CPU implementation in portable C89 manner

  Requires `tRomInit` to be implemented in the same translation unit as a macro
  ```c
  #define T_ROM_IN(mem, sz)
  ```
*/

#include "ttzn.h"
#include "tio.h"
#include "tasrt.h"

/* TODO Must be zerosegment on 6502 */
/* Registers */
T_U8 tCpuRgA;
T_U8 tCpuRgB;
T_U8 tCpuRgC;
T_U8 tCpuRgD;
T_U8 tCpuRgSL;
T_U8 tCpuRgSH;
T_U8 tCpuRgF; /* Flag Status Register */

T_U16 tCpuRgPC; /* 2 Program Counter 8-bit Registers */ /* TODO: Investigate whether `register` keyword would make gains here */

T_U8 tCpuRam[T_PG_N * T_PG_SZ]; /* RAM mapping */

T_U8 tCpuDvIn; /* Device intermediate, used for bypassing stack in device communication */

#include T_DVCS_C

void
tCpuInit(void)
{
  tLog("Zeroing RAM...\n");
  tMemSet(tCpuRam, 0x00, T_PG_N * T_PG_SZ);

  tLog("Zeroing registers...\n");
  tCpuRgA = 0x00;
  tCpuRgB = 0x00;
  tCpuRgC = 0x00;
  tCpuRgD = 0x00;
  tCpuRgSL = T_U16_LOW(T_SPINIT);
  tCpuRgSH = T_U16_HIGH(T_SPINIT);
  tCpuRgF = 0x00;
  tCpuRgPC = 0x00;

  tLog("Initializing cpu state...\n");
  T_ROM_IN(tCpuRam, T_PG_N * T_PG_SZ);

  tLog("Initializing devices...\n");
  tDvcInit();
}

T_NORET void tCpuExec(void);

/*
  This C implementation is used if target doesn't implement specific CPU execution model
  It's portable but potentially too slow and code heavy for some of targets
*/
#ifndef T_CPUASM
T_NORET
void
tCpuExec(void)
{
  tCpuInit();

  while (1)
  {
    switch (tCpuRam[tCpuRgPC++])
    {
      /* TODO We need alternative way of dispatching that would allow for memory savings
              And in general we need to test how our target compilers are optimizing it if at all

         TODO CC65 Produces individual comparison for each and every branch which is unacceptable
              We need to have jump table, in unique implementation
              https://www.nesdev.org/wiki/Jump_table
      */

      case tiMOV0A:
      {
        tCpuRgA = 0;
        break;
      }
      case tiMOVAB:
      {
        tCpuRgB = tCpuRgA;
        break;
      }
      case tiMOVAC:
      {
        tCpuRgC = tCpuRgA;
        break;
      }
      case tiMOVAD:
      {
        tCpuRgD = tCpuRgA;
        break;
      }
      case tiMOVASL:
      {
        tCpuRgSL = tCpuRgA;
        break;
      }
      case tiMOVASH:
      {
        tCpuRgSH = tCpuRgA;
        break;
      }
      case tiMOVAM:
      {
        tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)] = tCpuRgA;
        break;
      }
      case tiMOVBA:
      {
        tCpuRgA = tCpuRgB;
        break;
      }
      case tiMOVCA:
      {
        tCpuRgA = tCpuRgC;
        break;
      }
      case tiMOVDA:
      {
        tCpuRgA = tCpuRgD;
        break;
      }
      case tiMOVSLA:
      {
        tCpuRgA = tCpuRgSL;
        break;
      }
      case tiMOVSHA:
      {
        tCpuRgA = tCpuRgSH;
        break;
      }
      case tiMOVMA:
      {
        tCpuRgA = tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)];
        break;
      }
      case tiADDB:
      {
        tCpuRgA += tCpuRgB;
        tCpuRgF = tCpuRgA == T_U8_MIN;
        break;
      }
      case tiSUBB:
      {
        T_U8 res = tCpuRgA - tCpuRgB;
        tCpuRgF = res > tCpuRgA;
        tCpuRgA = res;
        break;
      }
      case tiMULB:
      {
        /* TODO Status flag needs testing */
        T_U8 res = tCpuRgA * tCpuRgB;
        tCpuRgF = ((tCpuRgA != 0) && ((res / tCpuRgA) != tCpuRgB));
        tCpuRgA = res;
        break;
      }
      case tiDIVB:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA / tCpuRgB;
        break;
      }
      case tiMODB:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA % tCpuRgB;
        break;
      }
      case tiINCA:
      {
        tCpuRgF = ++tCpuRgA == T_U8_MIN;
        break;
      }
      case tiDECA:
      {
        tCpuRgF = --tCpuRgA == T_U8_MAX;
        break;
      }
      case tiINCB:
      {
        tCpuRgF = ++tCpuRgB == T_U8_MIN;
        break;
      }
      case tiDECB:
      {
        tCpuRgF = --tCpuRgB == T_U8_MAX;
        break;
      }
      case tiINCBC:
      {
        if (++tCpuRgB == T_U8_MIN)
          tCpuRgF = ++tCpuRgC == T_U8_MIN;
        else
          tCpuRgF = 0x00;
        break;
      }
      case tiDECBC:
      {
        if (--tCpuRgB == T_U8_MAX)
          tCpuRgF = --tCpuRgC == T_U8_MAX;
        else
          tCpuRgF = 0x00;
        break;
      }
      case tiINCM:
      {
        ++tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)];
        break;
      }
      case tiDECM:
      {
        --tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)];
        break;
      }
      case tiEQLB:
      {
        tCpuRgF = tCpuRgA == tCpuRgB;
        break;
      }
      case tiCMPB:
      {
        tCpuRgF = tCpuRgA < tCpuRgB;
        break;
      }
      case tiJMPRA:
      {
        T_U8 rel_addr = tCpuRam[tCpuRgA];
        T_U16SI8(tCpuRgPC, rel_addr);
        break;
      }
      case tiJMPCRA:
      {
        T_U8 rel_addr = tCpuRam[tCpuRgA] * tCpuRgF;
        T_U16SI8(tCpuRgPC, rel_addr);
        break;
      }
      case tiDVCWA:
      {
        tCpuDvIn = tCpuRgA;
        tznDvcWr();
        break;
      }
      case tiDVCWM:
      {
        tCpuDvIn = tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)];
        tznDvcWr();
        break;
      }
      case tiDVCRA:
      {
        tznDvcRd();
        tCpuRgA = tCpuDvIn;
        break;
      }
      case tiDVCRM:
      {
        tznDvcRd();
        tCpuRam[T_U16_INIT(tCpuRgB, tCpuRgC)] = tCpuDvIn;
        break;
      }
      case tiCALLBC:
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
      case tiRET:
      {
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        T_U16_HSET(tCpuRgPC, tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)]);
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        T_U16_LSET(tCpuRgPC, tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)]);
        break;
      }
      case tiPUSHA:
      {
        tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)] = tCpuRgA;
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        break;
      }
      case tiPUSHB:
      {
        tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)] = tCpuRgB;
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        break;
      }
      case tiPOPA:
      {
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        tCpuRgA = tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)];
        break;
      }
      case tiPOPB:
      {
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        tCpuRgB = tCpuRam[T_U16_INIT(tCpuRgSL, tCpuRgSH)];
        break;
      }
      case tiFLP:
      {
        tCpuRgF ^= 0x01;
        break;
      }
      case tiCARTOL:
      {
        tCpuRgF = (tCpuRgF & 0xFE) | ((tCpuRgF >> 0x01) & 0x01);
        break;
      }
      case tiMOVIA:
      {
        tCpuRgA = tCpuRam[tCpuRgPC++];
        break;
      }
      case tiMOVIB:
      {
        tCpuRgB = tCpuRam[tCpuRgPC++];
        break;
      }
      case tiMOVIC:
      {
        tCpuRgC = tCpuRam[tCpuRgPC++];
        break;
      }
      case tiMOVID:
      {
        tCpuRgD = tCpuRam[tCpuRgPC++];
        break;
      }
      case tiMOVISL:
      {
        tCpuRgSL = tCpuRam[tCpuRgPC++];
        break;
      }
      case tiMOVISH:
      {
        tCpuRgSH = tCpuRam[tCpuRgPC++];
        break;
      }
      case tiADDI:
      {
        tCpuRgA += tCpuRam[tCpuRgPC++];
        tCpuRgF = tCpuRgA == T_U8_MIN;
        break;
      }
      case tiSUBI:
      {
        T_U8 res = tCpuRgA - tCpuRam[tCpuRgPC++];
        tCpuRgF = res > tCpuRgA;
        tCpuRgA = res;
        break;
      }
      case tiMULI:
      {
        /* TODO Status flag needs testing */
        T_U8 mul = tCpuRam[tCpuRgPC++];
        T_U8 res = tCpuRgA * mul;
        tCpuRgF = ((tCpuRgA != 0) && ((res / tCpuRgA) != mul));
        tCpuRgA = res;
        break;
      }
      case tiDIVI:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA / tCpuRam[tCpuRgPC++];
        break;
      }
      case tiMODI:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA % tCpuRam[tCpuRgPC++];
        break;
      }
      case tiEQLI:
      {
        tCpuRgF = tCpuRgA == tCpuRam[tCpuRgPC++];
        break;
      }
      case tiCMPI:
      {
        tCpuRgF = tCpuRgA < tCpuRam[tCpuRgPC++];
        break;
      }
      case tiJMPRI:
      {
        T_U8 rel_addr = tCpuRam[tCpuRgPC++];
        T_U16SI8(tCpuRgPC, rel_addr);
        break;
      }
      case tiJMPCRI:
      {
        T_U8 rel_addr = tCpuRam[tCpuRgPC++] * tCpuRgF;
        T_U16SI8(tCpuRgPC, rel_addr);
        break;
      }
      case tiDVCWI:
      {
        tCpuDvIn = tCpuRam[tCpuRgPC++];
        tznDvcWr();
        break;
      }
      case tiCALLRI:
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
      case tiMOVIMA:
      {
        tCpuRgA = tCpuRam[T_U16_INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])];
        tCpuRgPC += 2;
        break;
      }
      case tiMOVIAM:
      {
        tCpuRam[T_U16_INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])] = tCpuRgA;
        tCpuRgPC += 2;
        break;
      }
      case tiDVCRIM:
      {
        tznDvcRd();
        tCpuRam[T_U16_INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])] = tCpuDvIn;
        tCpuRgPC += 2;
        break;
      }
      case tiCALLI:
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
#endif
