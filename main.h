#ifndef SKIRMISH_MAIN
#define SKIRMISH_MAIN

#include "SDL.h"
#include <stdbool.h>

SDL_Surface *screen;
bool paused = false;
void toggle_pause();
bool ruby_enabled = false;

#endif
