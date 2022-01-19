/*
  System communication abstraction

  TODO
    - Abstract all of libc in here
    - Implement memset / memcpy 
*/

#ifndef TZN_SYSH
#define TZN_SYSH

#include "tznstd.h"

/* File operation modes */
#define TZN_FL0  0x00 /* Default */
#define TZN_FLIS 0x01 /* Ignore size */
#define TZN_FLRW 0x02 /* Rewrite */

/*
  @brief  Read contents of file at filename to memory buffer of specified size
          Flags dictate behaviour
  @params was_read -- Count of bytes read, optional
  @return Status code, 0 is success
*/
tznErr tznFlRd(const char* filename, U8* memory, U16 size, U16* was_read, U8 flags);

/*
  @brief  Write buffer to a file
*/
tznErr tznFlWr(const char* filename, U8* memory, U16 size, U8 flags);

#endif
