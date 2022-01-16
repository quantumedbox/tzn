#ifndef TZN_OPCODES_H
#define TZN_OPCODES_H

enum {
  ZEROA,  /* Zero A register */

  SETA,   /* Set A by immediate byte */
  SETB,   /* Set X by immediate byte */
  SETC,   /* Set X by immediate byte */
  SETD,   /* Set Y by immediate byte */

  MOVAB,  /* Move A to B */
  MOVAC,  /* Move A to C */
  MOVAD,  /* Move A to D */

  MOVBA,  /* Move B to A */
  MOVCA,  /* Move C to A */
  MOVDA,  /* Move D to A */
  MOVMA,  /* Move memory at [A B] to A */

  INCA,   /* Increment A, sets S to 0x01 on overflow */
  INCAB,  /* Increment A B as word, sets S to 0x01 on overflow */

  DECA,   /* Decrement A, sets S to 0x01 on underflow */
  DECAB,  /* Decrement A B as word, sets S to 0x01 on underflow */

  FLP,    /* Flip first bit of S register */

  ADDI,   /* Add immediate byte to A */
  ADDB,   /* Add B to A */

  SUBI,   /* Subtract immediate byte from A */
  SUBB,   /* Subtract B from A */

  EQLI,   /* Equality of A and immediate byte, S is set to 0x01 on equality and 0x00 otherwise */
  EQLB,   /* Equality of A and B, S is set to 0x01 on equality and 0x00 otherwise */

  JMPRI,  /* Relative unconditional jump by immediate byte treated as singed */
  JMPCRI, /* Relative conditional jump by immediate byte treated as singed */

  DVWI,   /* Device write from immediate byte where D is device register */
  DVWA,   /* Device write from A where D is device register */
  DVWM,   /* Device write from memory at [A B] where D is device register */

  DVR,    /* Device read to A where D is device register */
};

#endif
