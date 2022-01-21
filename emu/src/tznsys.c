#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tznsys.h"
#include "tznstd.h"

/* TODO Abstract IO operations for portability on various libc implementations (Plan 9 for example) */
/* TODO Move to shared TZN code, not EMU specific base */

void
tznPrint(const char* literal)
{
  fputs(literal, stdout);
}

void
tznMeSet(void* dest, U16 bytes, U8 value)
{
  memset(dest, value, (size_t)bytes);
}

void
tznMeCpy(void* dest, void* src, U16 bytes)
{
  memcpy(dest, src, (size_t)bytes);
}

static
TZN_BOOL
flExists(const char* filename)
{
  FILE* fp = fopen(filename, "r");
  if (fp)
  {
    fclose(fp);
    return TZN_YES;
  }
  return TZN_NO;
}

TZN_NORE
void
tznError(const char* literal)
{
  fputs("[error: ", stderr);
  fputs(literal, stderr);
  fputc(']', stderr);
  exit(EXIT_FAILURE);
}

/* TODO Probably a good idea to assert some ferror() checks */
TZN_ERR
tznFlRd(const char* filename, U8* memory, U16 size, U16* was_read, U8 flags)
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

  rdbt = fread(memory, sizeof(U8), (size_t)size, fp);
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

TZN_ERR
tznFlWr(const char* filename, U8* memory, U16 size, U8 flags)
{
  FILE* fp;
  size_t written;

  if (!(flags & TZN_FLRW) && (flExists(filename)))
    goto ERR;

  fp = fopen(filename, "wb");
  if (!fp)
    goto ERR;

  written = fwrite(memory, sizeof(U8), (size_t)size, fp);
  if (written != (size_t)size)
    goto ERR_FILE;

ERR_FILE:
  fclose(fp);
ERR:
  return 1;
}
