#include <stdio.h>

#include "tzn-cpu.h"
#include "tzn-std.h"
#include "tzn-device.h"

static U8 memory[TZN_MEMORY_BYTES];

enum { rgA, rgB, rgC, rgD, rgL, rgH };

void
tzn_Restart(void)
{
  U16 idx = TZN_MEMORY_BYTES;
  while (idx--)
    memory[idx] = 0;
}

void
tzn_WriteMemory(const U8* to_cpy, U16 offset, U16 size)
{
  TZN_ASSERT(((U32)offset + (U32)size) < (U32)TZN_MEMORY_BYTES, "memory write past RAM size");

  U16 idx = 0;
  for (; idx < size; idx++)
  {
    memory[offset + idx] = to_cpy[idx];
  }
}

void
tzn_Exec(void)
{
  tzn_DevicesInit();

  register U16 program_counter = TZN_MEMORY_RAM_START;
  U8 registers[TZN_GENERAL_REGISTER_COUNT] = {0};
  U8 status_register;

  registers[rgL] = U16_LOW(TZN_STARTUP_STACK_POINTER_VALUE);
  registers[rgH] = U16_HIGH(TZN_STARTUP_STACK_POINTER_VALUE);

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
        if (rel_addr & 0x80)
          program_counter = (program_counter - (U8)~rel_addr) - 1;
        else
          program_counter += rel_addr;
        break;
      }
      case JMPCRI:
      {
        U8 rel_addr = memory[program_counter++] * status_register;
        if (rel_addr & 0x80)
          program_counter = (program_counter - (U8)~rel_addr) - 1;
        else
          program_counter += rel_addr;
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
  }
  TZN_UNREACHABLE();
}
