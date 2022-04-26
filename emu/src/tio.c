#include "tio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ttzn.h"

/* TODO Abstract IO operations for portability on various libc implementations (Plan 9 for example) */
/* TODO Move to shared TZN code, not EMU specific base */
/* TODO Use <fcntl.h> instead, it provides simpler functions */

T_NORET
void
tznError(const char* literal)
{
  fputs("error: ", stderr);
  fputs(literal, stderr);
  exit(EXIT_FAILURE);
}

/* Returns true if file with given name exists, otherwise false */
static
T_BOOL
tFlExst(const char* filename)
{
  FILE* fp = fopen(filename, "r");
  if (fp)
  {
    fclose(fp);
    return T_TRUE;
  }
  return T_FALSE;
}

/* TODO Probably a good idea to assert some ferror() checks */
T_ERR
tznFlRd(const char* filename, T_U8* memory, T_U16 size, T_U16* was_read, T_U8 flags)
{
  FILE* fp;
  long int fp_size;
  size_t rdbt;

  fp = fopen(filename, "rb");

  if (!fp)
    goto ERR;

  #ifndef __CC65__
  if (fseek(fp, 0L, SEEK_END))
    goto ERR_FILE;

  fp_size = ftell(fp);
  if (fp_size == -1L)
    goto ERR_FILE;

  rewind(fp);
  #endif

  /* By default it is an error to have file that couldn't fit into buffer */
  if (!(flags & TZN_FLIS) && (fp_size > (long int)size))
    goto ERR_FILE;

  rdbt = fread(memory, sizeof(T_U8), (size_t)size, fp);
  if (rdbt != (size_t)fp_size)
    goto ERR_FILE; 

  if (was_read)
    *was_read = rdbt;

  fclose(fp);
  return 0;

ERR_FILE:
  fclose(fp);
ERR:
  return 1;
}

T_ERR
tznFlWr(const char* filename, T_U8* memory, T_U16 size, T_U8 flags)
{
  FILE* fp;
  size_t written;

  if (!(flags & TZN_FLRW) && (tFlExst(filename)))
    goto ERR;

  fp = fopen(filename, "wb");
  if (!fp)
    goto ERR;

  written = fwrite(memory, sizeof(T_U8), (size_t)size, fp);
  if (written != (size_t)size)
    goto ERR_FILE;

ERR_FILE:
  fclose(fp);
ERR:
  return 1;
}
