#ifndef SKIRMISH_RB_INTERFACE
#define SKIRMISH_RB_INTERFACE

#include "SDL.h"

void rb_interface_load();

void rb_interface_init(char *);
void rb_interface_render(SDL_Surface *);
void rb_interface_event(SDL_Event);
void rb_interface_cleanup();
void rb_interface_update();

#endif
