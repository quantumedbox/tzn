#ifndef TOPS_H
#define TOPS_H

/* TODO Do we need bit operators? Feels like shift left right might be quite handy */
/* TODO Indirect operations */
/* TODO
  We need to research actual use cases to determine how to make instruction set that would lead to producing minimal size code

  Problem #1:
    B is used in arithmetic and as address in pair [B C] which leads to need for constant storing and restoring
      Reduction of store/restore is one of the biggest ventures in which we can reduce the code size

    Solution #1:
      Add register for use as an arithmetic operand
*/

#define tiZEROA (unsigned char)0
#define tiMOVAB (unsigned char)1    /* Move A to B */
#define tiMOVAC (unsigned char)2    /* Move A to C */
#define tiMOVAD (unsigned char)3    /* Move A to D */
#define tiMOVASL (unsigned char)4   /* Move A to SL */
#define tiMOVASH (unsigned char)5   /* Move A to SH */
#define tiMOVAM (unsigned char)6    /* Move A to memory at [B C] */
#define tiMOVBA (unsigned char)7    /* Move B to A */
#define tiMOVCA (unsigned char)8    /* Move C to A */
#define tiMOVDA (unsigned char)9    /* Move D to A */
#define tiMOVSLA (unsigned char)10  /* Move SL to A */
#define tiMOVSHA (unsigned char)11  /* Move SH to A */
#define tiMOVMA (unsigned char)12   /* Move memory at [B C] to A */
#define tiADDB (unsigned char)13    /* Add B to A */
#define tiSUBB (unsigned char)14    /* Subtract B from A */
#define tiMULB (unsigned char)15    /* Multiply A and B */
#define tiDIVB (unsigned char)16    /* Divide A by B */
#define tiMODB (unsigned char)17    /* Modulo division of A by B */
#define tiINCA (unsigned char)18
#define tiINCB (unsigned char)19
#define tiINCBC (unsigned char)20
#define tiINCM (unsigned char)21
#define tiDECA (unsigned char)22
#define tiDECB (unsigned char)23
#define tiDECBC (unsigned char)24
#define tiDECM (unsigned char)25
#define tiEQLB (unsigned char)26    /* Equality of A and B, S is set to 0x01 on equality and 0x00 otherwise */
#define tiCMPB (unsigned char)27    /* Less than comparison between A and B, logic Status register is set if A is less than B */
#define tiJMPRA (unsigned char)28   /* Relative unconditional jump by A register treated as singed */
#define tiJMPCRA (unsigned char)29  /* Relative conditional jump by A register treated as singed */
#define tiCALLBC (unsigned char)30  /* Store [PL PH] at stack, jump to [B C] */
#define tiRET (unsigned char)31
#define tiPUSHA (unsigned char)32
#define tiPUSHB (unsigned char)33
#define tiPOPA (unsigned char)34
#define tiPOPB (unsigned char)35
#define tiFLP (unsigned char)36     /* Flip logic (first) bit of Status register */
#define tiCARTOL (unsigned char)37  /* Set logic (first) bit to value of second (carry / overflow) bit of Status register */
#define tiMOVIA (unsigned char)38   /* Move immediate byte to A */
#define tiMOVIB (unsigned char)39   /* Move immediate byte to B */
#define tiMOVIC (unsigned char)40   /* Move immediate byte to C */
#define tiMOVID (unsigned char)41   /* Move immediate byte to D */
#define tiMOVISL (unsigned char)42  /* Move immediate byte to SL */
#define tiMOVISH (unsigned char)43  /* Move immediate byte to SH */
#define tiADDI (unsigned char)44    /* Add immediate byte to A */
#define tiSUBI (unsigned char)45    /* Subtract immediate byte from A */
#define tiMULI (unsigned char)46
#define tiDIVI (unsigned char)47
#define tiMODI (unsigned char)48
#define tiEQLI (unsigned char)49    /* Equality of A and immediate byte, S is set to 0x01 on equality and 0x00 otherwise */
#define tiCMPI (unsigned char)50    /* Less than comparison between A and immediate byte, logic Status register is set if A is less than immediate byte */
#define tiJMPRI (unsigned char)51   /* Relative unconditional jump by immediate byte treated as singed */
#define tiJMPCRI (unsigned char)52  /* Relative conditional jump by immediate byte treated as singed */
#define tiCALLRI (unsigned char)53  /* Store address at [SL SH] and perform relative jump to immediate byte */
#define tiMOVIMA (unsigned char)54  /* Move memory at immediate absolute address to A */
#define tiMOVIAM (unsigned char)55  /* Move A to an absolute immediate address */
#define tiCALLI (unsigned char)56   /* Store address at [SL SH] and perform jump to immediate absolute byte */
#define tiFLSHI (unsigned char)57   /* Flush INT device */
#define tiOUTIA (unsigned char)58   /* Move byte from A to [0x00 INT], then flush INT device */
#define tiOUTIM (unsigned char)59   /* Move byte from [B C] to [0x00 INT], then flush INT device */
#endif
