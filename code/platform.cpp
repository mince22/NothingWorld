#include "platform.hpp"

#include <assert.h>

#include <SDL.h>
#include <SDL_syswm.h>

HWND hwnd;

void init(SDL2 *sdl2) {
	int result = SDL_Init(SDL_INIT_EVERYTHING);
	assert(result == 0);

	sdl2->window = SDL_CreateWindow("Nothing World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	assert(sdl2->window);

	SDL_SysWMinfo wm_info;
	SDL_VERSION(&wm_info.version);
	SDL_GetWindowWMInfo(sdl2->window, &wm_info);

	// do we need this? it works fine without setting hwnd for directx
	// this feels strange, review's needed.
	hwnd = wm_info.info.win.window;
}

void cleanup(SDL2 *sdl2) {
	SDL_DestroyWindow(sdl2->window);
	SDL_Quit();
}
