#include <stdio.h>

#include "tzncpu.h"
#include "tznstd.h"
#include "tznio.h"
#include "tzndvc.h"

static U8 shdl_res; /* Should Restart */
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
  shdl_res = 1;
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
      case ZEROA:
      {
        regs[rgA] = 0;
        break;
      }
      case SETA:
      {
        regs[rgA] = memory[pgc_r++];
        break;
      }
      case SETB:
      {
        regs[rgB] = memory[pgc_r++];
        break;
      }
      case SETC:
      {
        regs[rgC] = memory[pgc_r++];
        break;
      }
      case SETD:
      {
        regs[rgD] = memory[pgc_r++];
        break;
      }
      case MOVAB:
      {
        regs[rgB] = regs[rgA];
        break;
      }
      case MOVAC:
      {
        regs[rgC] = regs[rgA];
        break;
      }
      case MOVAD:
      {
        regs[rgD] = regs[rgA];
        break;
      }
      case MOVBA:
      {
        regs[rgA] = regs[rgB];
        break;
      }
      case MOVCA:
      {
        regs[rgA] = regs[rgC];
        break;
      }
      case MOVDA:
      {
        regs[rgA] = regs[rgD];
        break;
      }
      case MOVMA:
      {
        regs[rgA] = memory[U16_INIT(regs[rgB], regs[rgC])];
        break;
      }
      case INCA:
      {
        regs[rgA]++;
        status_r = regs[rgA] == U8_MIN;
        break;
      }
      case DECA:
      {
        regs[rgA]--;
        status_r = regs[rgA] == U8_MAX;
        break;
      }
      case INCBC:
      {
        if (++regs[rgB] == U8_MIN)
          status_r = ++regs[rgC] == U8_MIN;
        else
          status_r = 0x00;
        break;
      }
      case DECAB:
      {
        if (--regs[rgB] == U8_MAX)
          status_r = --regs[rgC] == U8_MAX;
        else
          status_r = 0x00;
        break;
      }
      case FLP:
      {
        status_r ^= 0x01;
        break;
      }
      case ADDI:
      {
        regs[rgA] += memory[pgc_r++];
        status_r = regs[rgA] == U8_MIN;
        break;
      }
      case ADDB:
      {
        regs[rgA] += regs[rgB];
        status_r = regs[rgA] == U8_MIN;
        break;
      }
      case SUBI:
      {
        U8 res = regs[rgA] - memory[pgc_r++];
        status_r = res > regs[rgA];
        regs[rgA] = res;
        break;
      }
      case SUBB:
      {
        U8 res = regs[rgA] - regs[rgB];
        status_r = res > regs[rgA];
        regs[rgA] = res;
        break;
      }
      case EQLI:
      {
        status_r = regs[rgA] == memory[pgc_r++];
        break;
      }
      case EQLB:
      {
        status_r = regs[rgA] == regs[rgB];
        break;
      }
      case JMPRI:
      {
        U8 rel_addr = memory[pgc_r++];
        U16_S_I8(pgc_r, rel_addr);
        break;
      }
      case JMPCRI:
      {
        U8 rel_addr = memory[pgc_r++] * status_r;
        U16_S_I8(pgc_r, rel_addr);
        break;
      }
      case DVWI:
      {
        tznDvcWr(memory[pgc_r++], regs[rgD]);
        break;
      }
      case DVWA:
      {
        tznDvcWr(regs[rgA], regs[rgD]);
        break;
      }
      case DVWM:
      {
        tznDvcWr(memory[U16_INIT(regs[rgB], regs[rgC])], regs[rgD]);
        break;
      }
      case DVR:
      {
        regs[rgA] = tznDvcRd(regs[rgD]);
        break;
      }
      default: {
        tznError("instruction not implemented");
      }
    }
    if (shdl_res) {
      tznCpuIn(memory);
      tznDvcIn();
      INIT_REGISTER_STATE();
      shdl_res = 0;
    }
  }
#undef INIT_REGISTER_STATE
}
