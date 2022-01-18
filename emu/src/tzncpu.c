#include <stdio.h>

#include "tzncpu.h"
#include "tznstd.h"
#include "tznio.h"
#include "tzndvc.h"

static U8 should_restart;
static CpuMemoryInitCallback memory_init_callback;

enum { rgA, rgB, rgC, rgD, rgL, rgH };

static
void
tzn_CpuInit(U8* memory)
{
  TZN_ASSERT(memory_init_callback != TZN_VOID, "NULL in memory_init_callback");
  memory_init_callback(&memory[TZN_MEMORY_RAM_START], TZN_MEMORY_RAM_BYTES);
}

void
tzn_CpuRestart(void)
{
  should_restart = 1;
}

void
tzn_CpuRegisterMemoryInitCallback(CpuMemoryInitCallback callback)
{
  TZN_ASSERT(callback != TZN_VOID, "NULL passed in tzn_CpuRegisterMemoryInitCallback()");
  memory_init_callback = callback;
}

TZN_NORETURN
void
tzn_CpuExec(void)
{
  /* TODO Only provide RAM? Not full address range */
  /*
    TODO Combine related field into handler to pass around,
         for example, that way we could have INIT_REGISTER_STATE as function instead of macro
  */
  U8 memory[TZN_MEMORY_BYTES];
  register U16 program_counter;
  U8 registers[TZN_GENERAL_REGISTER_COUNT];
  U8 status_register;

  tzn_CpuInit(memory);
  tzn_DevicesInit();

#define INIT_REGISTER_STATE() { \
    U16 idx = TZN_GENERAL_REGISTER_COUNT; \
    while (idx--) \
      registers[idx] = 0x00; \
    program_counter = TZN_MEMORY_RAM_START; \
    status_register = 0x00; \
    registers[rgL] = U16_LOW(TZN_STARTUP_STACK_POINTER_VALUE); \
    registers[rgH] = U16_HIGH(TZN_STARTUP_STACK_POINTER_VALUE); \
  }

  INIT_REGISTER_STATE();

  while (1)
  {
#ifdef TZN_DUMP_EXEC_STATE
    fprintf(
      stdout,
      "[PC: 0x%X | A: 0x%X | B: 0x%X | C: 0x%X | D: 0x%X | L: 0x%X | H: 0x%X | S: 0x%X]\n",
      program_counter,
      registers[rgA],
      registers[rgB],
      registers[rgC],
      registers[rgD],
      registers[rgL], /* TO-CONSIDER Combine L and H into one? Same as with PC */
      registers[rgH],
      status_register
    );
#endif
    switch (memory[program_counter++])
    {
      case ZEROA:
      {
        registers[rgA] = 0;
        break;
      }
      case SETA:
      {
        registers[rgA] = memory[program_counter++];
        break;
      }
      case SETB:
      {
        registers[rgB] = memory[program_counter++];
        break;
      }
      case SETC:
      {
        registers[rgC] = memory[program_counter++];
        break;
      }
      case SETD:
      {
        registers[rgD] = memory[program_counter++];
        break;
      }
      case MOVAB:
      {
        registers[rgB] = registers[rgA];
        break;
      }
      case MOVAC:
      {
        registers[rgC] = registers[rgA];
        break;
      }
      case MOVAD:
      {
        registers[rgD] = registers[rgA];
        break;
      }
      case MOVBA:
      {
        registers[rgA] = registers[rgB];
        break;
      }
      case MOVCA:
      {
        registers[rgA] = registers[rgC];
        break;
      }
      case MOVDA:
      {
        registers[rgA] = registers[rgD];
        break;
      }
      case MOVMA:
      {
        registers[rgA] = memory[U16_COMPOSE(registers[rgB], registers[rgC])];
        break;
      }
      case INCA:
      {
        registers[rgA]++;
        status_register = registers[rgA] == U8_MIN;
        break;
      }
      case DECA:
      {
        registers[rgA]--;
        status_register = registers[rgA] == U8_MAX;
        break;
      }
      case INCBC:
      {
        if (++registers[rgB] == U8_MIN)
          status_register = ++registers[rgC] == U8_MIN;
        else
          status_register = 0x00;
        break;
      }
      case DECAB:
      {
        if (--registers[rgB] == U8_MAX)
          status_register = --registers[rgC] == U8_MAX;
        else
          status_register = 0x00;
        break;
      }
      case FLP:
      {
        status_register ^= 0x01;
        break;
      }
      case ADDI:
      {
        registers[rgA] += memory[program_counter++];
        status_register = registers[rgA] == U8_MIN;
        break;
      }
      case ADDB:
      {
        registers[rgA] += registers[rgB];
        status_register = registers[rgA] == U8_MIN;
        break;
      }
      case SUBI:
      {
        U8 res = registers[rgA] - memory[program_counter++];
        status_register = res > registers[rgA];
        registers[rgA] = res;
        break;
      }
      case SUBB:
      {
        U8 res = registers[rgA] - registers[rgB];
        status_register = res > registers[rgA];
        registers[rgA] = res;
        break;
      }
      case EQLI:
      {
        status_register = registers[rgA] == memory[program_counter++];
        break;
      }
      case EQLB:
      {
        status_register = registers[rgA] == registers[rgB];
        break;
      }
      case JMPRI:
      {
        U8 rel_addr = memory[program_counter++];
        U16_SUB_I8(program_counter, rel_addr);
        break;
      }
      case JMPCRI:
      {
        U8 rel_addr = memory[program_counter++] * status_register;
        U16_SUB_I8(program_counter, rel_addr);
        break;
      }
      case DVWI:
      {
        tzn_DeviceWrite(memory[program_counter++], registers[rgD]);
        break;
      }
      case DVWA:
      {
        tzn_DeviceWrite(registers[rgA], registers[rgD]);
        break;
      }
      case DVWM:
      {
        tzn_DeviceWrite(memory[U16_COMPOSE(registers[rgB], registers[rgC])], registers[rgD]);
        break;
      }
      case DVR:
      {
        registers[rgA] = tzn_DeviceRead(registers[rgD]);
        break;
      }
      default: {
        tzn_Error("instruction not implemented");
      }
    }
    if (should_restart) {
      tzn_CpuInit(memory);
      tzn_DevicesInit();
      INIT_REGISTER_STATE();
      should_restart = 0;
    }
  }
#undef INIT_REGISTER_STATE
}
