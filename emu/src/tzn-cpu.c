#include <stdio.h>

#include "tzn-cpu.h"
#include "tzn-std.h"
#include "tzn-device.h"

/* TODO Only provide RAM? Not full address range */
static U8 memory[TZN_MEMORY_BYTES];
static U8 should_restart;

/* Passed externally, should outlive next tzn_CpuExec call which depends on it */
static const U8* init_memory = TZN_VOID;
static U16 init_memory_size;

enum { rgA, rgB, rgC, rgD, rgL, rgH };

void
tzn_Restart(void)
{
  should_restart = 1;
}

static
void
tzn_CpuInit(void)
{
  U16 idx;

  TZN_ASSERT(init_memory != TZN_VOID, "void pointer for memory initialization, check that you're calling tzn_CpuPassInitMemory()");
  TZN_ASSERT(init_memory_size < TZN_MEMORY_RAM_BYTES, "cannot fit initialization memory");

  idx = 0;
  for (; idx < init_memory_size; idx++)
  {
    memory[TZN_MEMORY_RAM_START + idx] = init_memory[idx];
  }
}

void
tzn_CpuPassInitMemory(const U8* mem, U16 size)
{
  TZN_ASSERT(size < TZN_MEMORY_RAM_BYTES, "cannot fit initialization memory");

  init_memory = mem;
  init_memory_size = size;
}

TZN_NORETURN
void
tzn_CpuExec(void)
{
  register U16 program_counter;
  U8 registers[TZN_GENERAL_REGISTER_COUNT];
  U8 status_register;

  tzn_CpuInit();
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
      tzn_CpuInit();
      tzn_DevicesInit();
      INIT_REGISTER_STATE();
      should_restart = 0;
    }
  }
#undef INIT_REGISTER_STATE
}
