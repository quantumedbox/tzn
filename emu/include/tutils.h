#ifndef TUTILS_H
#define TUTILS_H

/* TODO
  Side-effect macros might cause problems
  Better swap them to pure functions
*/
/* TODO Deprecate fully?
  #define T_U16INIT(low, high) (((T_U16)(high) << 8) | (T_U16)(low))
  #define T_U16LOW(v) ((v) & 0x00FF)
  #define T_U16HIGH(v) (((v) & 0xFF00) >> 8)
  #define T_U16SETL(v, byte) do { v = ((v) & 0xFF00) | (byte); } while (0)
  #define T_U16SETH(v, byte) do { v = ((v) & 0x00FF) | ((byte) << 8); } while (0)
*/

/* Subtraction of signed 8 bit integer from 16 unsigned one */
#define T_U16SI8(sbtrhnd, sbtrctr) \
  if ((sbtrctr) & 0x80) \
    (sbtrhnd) = ((sbtrhnd) - (T_U8)~(sbtrctr)) - 1; \
  else \
    (sbtrhnd) += (sbtrctr);

#define T_STREX_(x) #x
#define T_STREXP(x) T_STREX_(x)

#endif
