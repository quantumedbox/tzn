#ifndef TZN_OPCODES_H
#define TZN_OPCODES_H

/* TODO Shouldn't be holey, but for now it is */
/* TODO Do we need bit operators? Feels like shift left right might be quite handy */
enum {
  /* First 64 instuctions are without immediate bytes */
  iMOV0A,
  iMOVAB,   /* Move A to B */
  iMOVAC,   /* Move A to C */
  iMOVAD,   /* Move A to D */
  iMOVASL,  /* Move A to SL */
  iMOVASH,  /* Move A to SH */
/*  iGAP,*/
  iMOVAM,   /* Move A to memory at [B C] */

/*  iGAP,*/ /* TODO Some special handy A register operation */
  iMOVBA,   /* Move B to A */
  iMOVCA,   /* Move C to A */
  iMOVDA,   /* Move D to A */
  iMOVSLA,  /* Move SL to A */
  iMOVSHA,  /* Move SH to A */
/*  iGAP,*/
  iMOVMA,   /* Move memory at [B C] to A */

  iADDB,    /* Add B to A */
  iSUBB,    /* Subtract B from A */
  iMULB,    /* Multiply A and B */
  iDIVB,    /* Divide A by B */
  iMODB,    /* Modulo division of A by B */
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/

  iINCA,
  iINCB,
  iINCBC,
  iINCM,
  iDECA,
  iDECB,
  iDECBC,
  iDECM,

  iEQLB,    /* Equality of A and B, S is set to 0x01 on equality and 0x00 otherwise */
  iCMPB,    /* Less than comparison between A and B, logic Status register is set if A is less than B */

  iJMPRA,   /* Relative unconditional jump by A register treated as singed */
  iJMPCRA,  /* Relative conditional jump by A register treated as singed */

  iDVCWA,   /* Device write to D port where A register contains data to send */
  iDVCWM,   /* Device write to D port where byte at memory location [B C] is to send */
  iDVCRA,   /* Device read from D port to A register */
  iDVCRM,   /* Device read from D port to [B C] memory address */

  iCALLBC,  /* Store [PL PH] at stack, jump to [B C] */
  iRET,

  iPUSHA,
  iPUSHB,
  iPOPA,
  iPOPB,

  iFLP,     /* Flip logic (first) bit of Status register */
  iCARTOL,  /* Set logic (first) bit to value of second (carry / overflow) bit of Status register */

/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/

/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/

  /* Next 64 instructions have 1 immediate byte */
  iMOVIA,   /* Move immediate byte to A */
  iMOVIB,   /* Move immediate byte to B */
  iMOVIC,   /* Move immediate byte to C */
  iMOVID,   /* Move immediate byte to D */
  iMOVISL,  /* Move immediate byte to SL */
  iMOVISH,  /* Move immediate byte to SH */
/*  iGAP,*/
/*  iGAP,*/

/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/


  iADDI,    /* Add immediate byte to A */
  iSUBI,    /* Subtract immediate byte from A */
  iMULI,
  iDIVI,
  iMODI,
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/

  /* TODO Inc / Dec by immediate? */
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/

  iEQLI,    /* Equality of A and immediate byte, S is set to 0x01 on equality and 0x00 otherwise */
  iCMPI,    /* Less than comparison between A and immediate byte, logic Status register is set if A is less than immediate byte */

  iJMPRI,   /* Relative unconditional jump by immediate byte treated as singed */
  iJMPCRI,  /* Relative conditional jump by immediate byte treated as singed */

  iDVCWI,   /* Device write to D port where immediate byte is to send */
/*  iGAP,*/

/*  iGAP,*/
/*  iGAP,*/

  iCALLRI,  /* Store address at [SL SH] and perform relative jump to immediate byte */
/*  iGAP,*/

/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/

/*  iGAP,*/
/*  iGAP,*/

/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/

/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/
/*  iGAP,*/

  /* Next 64 instructions have 2 immediate bytes */
  iMOVIMA,  /* Move memory at immediate absolute address to A */
  iMOVIAM,  /* Move A to an absolute immediate address */
  iDVCRIM,  /* Device read from D port to immediate absolute memory address */
  iCALLI    /* Store address at [SL SH] and perform jump to immediate absolute byte */

  /* TODO Fill this space */

  /* Next free estate for expansion sets */
};

#endif
