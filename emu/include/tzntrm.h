#ifndef TZN_TRMH
#define TZN_TRMH

#include "tznstd.h"

#define TZN_TRMR 0x00 /* Reset */
#define TZN_TRMX 0x10 /* Set Cursor X */
#define TZN_TRMY 0x11 /* Set Cursor Y */
#define TZN_TRMV 0x12 /* Set Cursor Visibility */
#define TZN_TRMC 0x13 /* Put Char */
#define TZN_TRMS 0x14 /* Put String */
#define TZN_TRMD 0x20 /* Get Display Size */
#define TZN_TRMA 0x21 /* Get Char At (X, Y) */

#endif
