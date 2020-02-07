#pragma once

#include <SDL.h>
#include <SDL_syswm.h>

struct SDL2 {
	SDL_Window *window;
};

void init(SDL2 *sdl2);
void cleanup(SDL2 *sdl2);