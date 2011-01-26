#ifndef SKIRMISH_EVENTS
#define SKIRMISH_EVENTS

#include "SDL.h"
#include "camera.h"
#include "display.h"

SDL_Event event;
void poll_for_events(camera *);
void handle_keypress(int, camera *);
void handle_mousedown(SDL_MouseButtonEvent, camera *);
void handle_mousemove(SDL_MouseMotionEvent);

#endif
