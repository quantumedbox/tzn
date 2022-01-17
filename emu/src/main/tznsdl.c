#include <stdlib.h>

/* TODO */

/*
#define SDL_MAIN_HANDLED
#include <SDL.h>
*/

#include "include/tzn-cpu.h"
#include "include/tzn-std.h"

/*
struct Handle
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Surface* surface;
  SDL_Texture* tex;
};

static struct Handle handle;

static void
SdlDeinit(void)
{
  SDL_DestroyRenderer(handle.renderer);
  SDL_DestroyWindow(handle.window);
  SDL_Quit();
}
*/

int
main(void)
{
  U8 test_code[] = {
    // SETX, 0x01, DVWI, 0x12, DVWI, 0x00, DVWI, 0x13, DVWI, 'H', JRI, -12
    SETX, 0x01, DVWI, 0x13, DVWI, 'T', JRI, -6
    /* 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0 */
  };

  tzn_WriteMemory(test_code, TZN_MEMORY_RAM_START, sizeof test_code);
  tzn_CpuExec();

  /*
  if (SDL_Init(SDL_INIT_VIDEO))
    tzn_Error("Error initializing SDL");

  if (!(handle.window = SDL_CreateWindow(
      "tzn-emu",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      TZN_DISPLAY_PIXEL_WIDTH,
      TZN_DISPLAY_PIXEL_HEIGHT,
      SDL_WINDOW_SHOWN)))
    tzn_Error("Error creating window");

  if (!(handle.renderer = SDL_CreateRenderer(handle.window, -1, SDL_RENDERER_ACCELERATED)))
    tzn_Error("Error creating renderer");

  if (atexit(SdlDeinit))
    tzn_Error("Error setting atexit");
  */
  return 0;
}
