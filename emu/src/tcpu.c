/*
  TZN CPU implementation in portable C89 manner

  Requires `T_ROM_IN` to be implemented in the same translation unit as a macro
  ```c
  #define T_ROM_IN(mem, sz)
  ```
*/

/* TODO Needs to be absolutely reworked */

#include "ttzn.h"
#include "tio.h"
#include "tasrt.h"

/* Needed to get around alignment issues */
typedef union {
  T_U16 whole;
  struct {
  #if defined(TZN_LEND)
    T_U8 low;
    T_U8 high;
  #else
    T_U8 high;
    T_U8 low;
  #endif
  } byte;
} T_U16ARR;

/* TODO Must be zerosegment on 6502, but looks like it would require messing with linker */
static T_U8 tCpuRgA = 0x00;
static T_U16ARR tCpuRgBC = { 0x0000 };
static T_U8 tCpuRgD = 0x00;
static T_U16ARR tCpuRgSP = { 0x0000 }; /* Stack Pointer */
static T_U8 tCpuRgF = 0x00; /* Flag Status Register */
static T_U16ARR tCpuRgPC = { 0x0100 }; /* Program Counter */
static T_U8 tCpuRam[T_PG_N * T_PG_SZ]; /* RAM mapping */
static T_U8 tCpuTemp = 0x00; /* Device intermediate, used for bypassing stack in device communications */

#include T_DVCS_C

/*
  This C implementation is used if target doesn't implement specific CPU execution model
  It's rather portable but potentially too slow and code heavy for some targets
*/
#ifndef T_CPUASM

T_NORET
void
tCpuExec(void)
{
  tLog("Initializing cpu state...\n");
  T_ROM_IN(&tCpuRam[0x0100], (T_PG_N - 1) * T_PG_SZ);

  T_CTRIN();

  /* PC and tCpuTemp are zeroed on startup and thus will not change state */
  PC_RELJMP:
    T_U16SI8(tCpuRgPC.whole, tCpuTemp);

  while (1)
  {
    #if defined(TZN_DUMP_EXEC_STATE)
      printf("[A: %d, BC: %d, D: %d, PC: %d, OP: %d]\n", tCpuRgA, tCpuRgBC.whole, tCpuRgD, tCpuRgPC.whole, tCpuRam[tCpuRgPC.whole]);
    #endif
    switch (tCpuRam[tCpuRgPC.whole++])
    {
      case tiZEROA:
      {
        tCpuRgA = 0;
        break;
      }
      case tiMOVAB:
      {
        tCpuRgBC.byte.low = tCpuRgA;
        break;
      }
      case tiMOVAC:
      {
        tCpuRgBC.byte.high = tCpuRgA;
        break;
      }
      case tiMOVAD:
      {
        tCpuRgD = tCpuRgA;
        break;
      }
      case tiMOVASL:
      {
        tCpuRgSP.byte.high = tCpuRgA;
        break;
      }
      case tiMOVASH:
      {
        tCpuRgSP.whole = tCpuRgA;
        break;
      }
      case tiMOVAM:
      {
        tCpuRam[tCpuRgBC.whole] = tCpuRgA;
        break;
      }
      case tiMOVBA:
      {
        tCpuRgA = tCpuRgBC.byte.low;
        break;
      }
      case tiMOVCA:
      {
        tCpuRgA = tCpuRgBC.byte.high;
        break;
      }
      case tiMOVDA:
      {
        tCpuRgA = tCpuRgD;
        break;
      }
      case tiMOVSLA:
      {
        tCpuRgA = tCpuRgSP.byte.high;
        break;
      }
      case tiMOVSHA:
      {
        tCpuRgA = tCpuRgSP.whole;
        break;
      }
      case tiMOVMA:
      {
        tCpuRgA = tCpuRam[tCpuRgBC.whole];
        break;
      }
      case tiADDB:
      {
        tCpuRgA += tCpuRgBC.byte.low;
        tCpuRgF = tCpuRgA == T_U8_MIN;
        break;
      }
      case tiSUBB:
      {
        T_U8 res = tCpuRgA - tCpuRgBC.byte.low;
        tCpuRgF = res > tCpuRgA;
        tCpuRgA = res;
        break;
      }
      case tiMULB:
      {
        /* TODO Status flag needs testing */
        /* TODO Potentially extremely slow, especially for compilers such as CC65 and SDCC */
        T_U8 res = tCpuRgA * tCpuRgBC.byte.low;
        tCpuRgF = ((tCpuRgA != 0) && ((res / tCpuRgA) != tCpuRgBC.byte.low));
        tCpuRgA = res;
        break;
      }
      case tiDIVB:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA / tCpuRgBC.byte.low;
        break;
      }
      case tiMODB:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA % tCpuRgBC.byte.low;
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
        tCpuRgF = ++tCpuRgBC.byte.low == T_U8_MIN;
        break;
      }
      case tiDECB:
      {
        tCpuRgF = --tCpuRgBC.byte.low == T_U8_MAX;
        break;
      }
      case tiINCBC:
      {
        ++tCpuRgBC.whole;
        break;
      }
      case tiDECBC:
      {
        --tCpuRgBC.whole;
        break;
      }
      case tiINCM:
      {
        ++tCpuRam[tCpuRgBC.whole];
        break;
      }
      case tiDECM:
      {
        --tCpuRam[tCpuRgBC.whole];
        break;
      }
      case tiEQLB:
      {
        tCpuRgF = tCpuRgA == tCpuRgBC.byte.low;
        break;
      }
      case tiCMPB:
      {
        tCpuRgF = tCpuRgA < tCpuRgBC.byte.low;
        break;
      }
      case tiJMPRA:
      {
        tCpuTemp = tCpuRam[tCpuRgA];
        goto PC_RELJMP;
      }
      case tiJMPCRA:
      {
        tCpuTemp = tCpuRam[tCpuRgA] * tCpuRgF;
        goto PC_RELJMP;
      }
      case tiCALLBC:
      {
        /* Stack pointer should always point to free to rewrite spot */
        /* TODO Assert for SP underflow on debug */
        tCpuRam[tCpuRgSP.whole] = tCpuRgPC.byte.low;
        --tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MAX, "Stack Pointer underflow");
        tCpuRam[tCpuRgSP.whole] = tCpuRgPC.byte.high;
        --tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MAX, "Stack Pointer underflow");
        tCpuRgPC.whole = tCpuRgBC.whole;
        break;
      }
      case tiRET:
      {
        ++tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MIN, "Stack Pointer overflow");
        tCpuRgPC.byte.high = tCpuRam[tCpuRgSP.whole];
        ++tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MIN, "Stack Pointer overflow");
        tCpuRgPC.byte.low = tCpuRam[tCpuRgSP.whole];
        break;
      }
      case tiPUSHA:
      {
        tCpuRam[tCpuRgSP.whole] = tCpuRgA;
        --tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MAX, "Stack Pointer underflow");
        break;
      }
      case tiPUSHB:
      {
        tCpuRam[tCpuRgSP.whole] = tCpuRgBC.byte.low;
        --tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MAX, "Stack Pointer underflow");
        break;
      }
      case tiPOPA:
      {
        ++tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MIN, "Stack Pointer overflow");
        tCpuRgA = tCpuRam[tCpuRgSP.whole];
        break;
      }
      case tiPOPB:
      {
        ++tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MIN, "Stack Pointer overflow");
        tCpuRgBC.byte.low = tCpuRam[tCpuRgSP.whole];
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
        tCpuRgA = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        break;
      }
      case tiMOVIB:
      {
        tCpuRgBC.byte.low = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        break;
      }
      case tiMOVIC:
      {
        tCpuRgBC.byte.high = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        break;
      }
      case tiMOVID:
      {
        tCpuRgD = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        break;
      }
      case tiMOVISL:
      {
        tCpuRgSP.byte.high = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        break;
      }
      case tiMOVISH:
      {
        tCpuRgSP.whole = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        break;
      }
      case tiADDI:
      {
        tCpuRgA += tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        tCpuRgF = tCpuRgA == T_U8_MIN;
        break;
      }
      case tiSUBI:
      {
        T_U8 res = tCpuRgA - tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        tCpuRgF = res > tCpuRgA;
        tCpuRgA = res;
        break;
      }
      case tiMULI:
      {
        /* TODO Status flag needs testing */
        T_U8 mul = tCpuRam[tCpuRgPC.whole];
        T_U8 res = tCpuRgA * mul;
        ++tCpuRgPC.whole;
        tCpuRgF = ((tCpuRgA != 0) && ((res / tCpuRgA) != mul));
        tCpuRgA = res;
        break;
      }
      case tiDIVI:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA / tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        break;
      }
      case tiMODI:
      {
        /* TODO Handle division by zero */
        tCpuRgA = tCpuRgA % tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        break;
      }
      case tiEQLI:
      {
        tCpuRgF = tCpuRgA == tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        break;
      }
      case tiCMPI:
      {
        tCpuRgF = tCpuRgA < tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        break;
      }
      case tiJMPRI:
      {
        tCpuTemp = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        goto PC_RELJMP;
      }
      case tiJMPCRI:
      {
        tCpuTemp = tCpuRam[tCpuRgPC.whole] * tCpuRgF;
        ++tCpuRgPC.whole;
        goto PC_RELJMP;
      }
      case tiCALLRI:
      {
        tCpuTemp = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        tCpuRam[tCpuRgSP.whole] = tCpuRgPC.byte.low;
        --tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MAX, "Stack Pointer underflow");
        tCpuRam[tCpuRgSP.whole] = tCpuRgPC.byte.high;
        --tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MAX, "Stack Pointer underflow");
        goto PC_RELJMP;
      }
      case tiMOVIMA:
      {
        tCpuRgA = tCpuRam[tCpuRam[tCpuRgPC.whole]];
        tCpuRgPC.whole += 2;
        break;
      }
      case tiMOVIAM:
      {
        tCpuRam[tCpuRam[tCpuRgPC.whole]] = tCpuRgA;
        tCpuRgPC.whole += 2;
        break;
      }
      case tiCALLI:
      {
        /* TODO Doesn't make sense, we need 2 immediate bytes here for specifying called addr */
        tCpuTemp = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        tCpuRam[tCpuRgSP.whole] = tCpuRgPC.byte.low;
        --tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MAX, "Stack Pointer underflow");
        tCpuRam[tCpuRgSP.whole] = tCpuRgPC.byte.high;
        --tCpuRgSP.whole;
        T_ASSERT(tCpuRgSP.whole != T_U16MAX, "Stack Pointer underflow");
        tCpuRgPC.whole = tCpuTemp;
        break;
      }
      case tiFLSHI:
      {
        tCpuTemp = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        tDvcFlsh();
        break;
      }
      case tiOUTIA:
      {
        tCpuTemp = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        tCpuRam[tCpuTemp] = tCpuRgA;
        tDvcFlsh();
        break;
      }
      case tiOUTIM:
      {
        tCpuTemp = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        tCpuRam[tCpuTemp] = tCpuRam[tCpuRgBC.whole];
        /*printf("dvc: %d, addr: %d, val: %d\n", tCpuTemp, tCpuRgBC.whole, tCpuRam[tCpuRgBC.whole]);*/
        tDvcFlsh();
        break;
      }
      case tiOUTII:
      {
        tCpuTemp = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        tCpuRam[tCpuTemp] = tCpuRam[tCpuRgPC.whole];
        ++tCpuRgPC.whole;
        tDvcFlsh();
        break;
      }
      default: {
        T_ASSERT(0, "Instruction not implemented");
      }
    }
  }
  TZN_DEAD();
}
#endif
