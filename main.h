#ifndef SKIRMISH_MAIN
#define SKIRMISH_MAIN

#include "SDL.h"
#include <stdbool.h>

SDL_Surface *screen;
bool paused = false;
void toggle_pause();
extern ui_state *current_state;

#endif
