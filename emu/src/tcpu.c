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

/* TODO Must be zerosegment on 6502, but looks like it would require messing with linker */
T_U8 tCpuRgA;

T_U8 tCpuRgBC[2];
#define tCpuRgB tCpuRgBC[0]
#define tCpuRgC tCpuRgBC[1]

T_U8 tCpuRgD;

T_U8 tCpuRgS[2];
#define tCpuRgSL tCpuRgS[0]
#define tCpuRgSH tCpuRgS[1]

T_U8 tCpuRgF; /* Flag Status Register */

T_U16 tCpuRgPC; /* 2 Program Counter 8-bit Registers */

T_U8 tCpuRam[T_PG_N * T_PG_SZ]; /* RAM mapping */

/* TODO Rename to reflect that it's general temporary, not just device one */
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
  tCpuRgSL = T_U16LOW(T_SPINIT);
  tCpuRgSH = T_U16HIGH(T_SPINIT);
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

  /* PC and DvIn are zeroed on startup and thus will not change state */
  PC_RELJMP:
    T_U16SI8(tCpuRgPC, tCpuDvIn);

  while (1)
  {
    switch (tCpuRam[tCpuRgPC++])
    {
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
        tCpuRam[(T_U16)tCpuRgB] = tCpuRgA;
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
        tCpuRgA = tCpuRam[(T_U16)tCpuRgB];
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
        ++tCpuRam[(T_U16)tCpuRgB];
        break;
      }
      case tiDECM:
      {
        --tCpuRam[(T_U16)tCpuRgB];
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
        tCpuDvIn = tCpuRam[tCpuRgA];
        goto PC_RELJMP;
      }
      case tiJMPCRA:
      {
        tCpuDvIn = tCpuRam[tCpuRgA] * tCpuRgF;
        goto PC_RELJMP;
      }
      case tiDVCWA:
      {
        tCpuDvIn = tCpuRgA;
        tznDvcWr();
        break;
      }
      case tiDVCWM:
      {
        tCpuDvIn = tCpuRam[(T_U16)tCpuRgB];
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
        tCpuRam[(T_U16)tCpuRgB] = tCpuDvIn;
        break;
      }
      case tiCALLBC:
      {
        /* Stack pointer should always point to free to rewrite spot */
        /* TODO Assert for SP underflow on debug */
        tCpuRam[(T_U16)tCpuRgSL] = T_U16LOW(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        tCpuRam[(T_U16)tCpuRgSL] = T_U16HIGH(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        tCpuRgPC = (T_U16)tCpuRgB;
        break;
      }
      case tiRET:
      {
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        T_U16SETH(tCpuRgPC, tCpuRam[(T_U16)tCpuRgSL]);
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        T_U16SETL(tCpuRgPC, tCpuRam[(T_U16)tCpuRgSL]);
        break;
      }
      case tiPUSHA:
      {
        tCpuRam[(T_U16)tCpuRgSL] = tCpuRgA;
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        break;
      }
      case tiPUSHB:
      {
        tCpuRam[(T_U16)tCpuRgSL] = tCpuRgB;
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        break;
      }
      case tiPOPA:
      {
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        tCpuRgA = tCpuRam[(T_U16)tCpuRgSL];
        break;
      }
      case tiPOPB:
      {
        if (++tCpuRgSL == T_U8_MIN)
          ++tCpuRgSH;
        tCpuRgB = tCpuRam[(T_U16)tCpuRgSL];
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
        tCpuDvIn = tCpuRam[tCpuRgPC++];
        goto PC_RELJMP;
      }
      case tiJMPCRI:
      {
        tCpuDvIn = tCpuRam[tCpuRgPC++] * tCpuRgF;
        goto PC_RELJMP;
      }
      case tiDVCWI:
      {
        tCpuDvIn = tCpuRam[tCpuRgPC++];
        tznDvcWr();
        break;
      }
      case tiCALLRI:
      {
        tCpuDvIn = tCpuRam[tCpuRgPC++];
        tCpuRam[(T_U16)tCpuRgSL] = T_U16LOW(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        tCpuRam[(T_U16)tCpuRgSL] = T_U16HIGH(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        goto PC_RELJMP;
      }
      case tiMOVIMA:
      {
        tCpuRgA = tCpuRam[T_U16INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])];
        tCpuRgPC += 2;
        break;
      }
      case tiMOVIAM:
      {
        tCpuRam[T_U16INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])] = tCpuRgA;
        tCpuRgPC += 2;
        break;
      }
      case tiDVCRIM:
      {
        tznDvcRd();
        tCpuRam[T_U16INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])] = tCpuDvIn;
        tCpuRgPC += 2;
        break;
      }
      case tiCALLI:
      {
        tCpuRam[(T_U16)tCpuRgSL] = T_U16LOW(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        tCpuRam[(T_U16)tCpuRgSL] = T_U16HIGH(tCpuRgPC);
        if (--tCpuRgSL == T_U8_MAX)
          --tCpuRgSH;
        tCpuRgPC = tCpuRam[T_U16INIT(tCpuRam[tCpuRgPC], tCpuRam[tCpuRgPC + 1])];
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
