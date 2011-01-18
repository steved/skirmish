#ifndef SKIRMISH_EVENTS
#define SKIRMISH_EVENTS

#include "SDL.h"

SDL_Event event;
void poll_for_events();
void handle_keypress(int key);

#endif
