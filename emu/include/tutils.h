#ifndef TUTILS_H
#define TUTILS_H

/* TODO
  Side-effect macros might cause problems
  Better swap them to pure functions
*/

#define T_U16_INIT(low, high) (((T_U16)(high) << 8) | (T_U16)(low))
#define T_U16_LOW(v) ((v) & 0x00FF)
#define T_U16_HIGH(v) (((v) & 0xFF00) >> 8)
/* Set low byte */
#define T_U16_LSET(v, byte) do { v = ((v) & 0xFF00) | (byte); } while (0)
/* Set high byte */
#define T_U16_HSET(v, byte) do { v = ((v) & 0x00FF) | ((byte) << 8); } while (0)

/* Subtraction of signed 8 bit integer from 16 unsigned one */
#define T_U16SI8(sbtrhnd, sbtrctr) \
  if ((sbtrctr) & 0x80) \
    (sbtrhnd) = ((sbtrhnd) - (T_U8)~(sbtrctr)) - 1; \
  else \
    (sbtrhnd) += (sbtrctr);

#define T_STREX_(x) #x
#define T_STREXP(x) T_STREX_(x)

#endif
