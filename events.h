#ifndef SKIRMISH_EVENTS
#define SKIRMISH_EVENTS

#include "SDL.h"
#include "zoom.h"

SDL_Event event;
void poll_for_events();
void handle_keypress(int key);
void handle_mousedown(int button, int x, int y);

#endif
