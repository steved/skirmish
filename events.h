#ifndef SKIRMISH_EVENTS
#define SKIRMISH_EVENTS

#include "SDL.h"
#include "camera.h"
#include "display.h"

SDL_Event event;
void poll_for_events(camera *camera);
void handle_keypress(int key, camera *camera);
void handle_mousedown(SDL_MouseButtonEvent button);
void handle_mousemove(SDL_MouseMotionEvent motion);

#endif
