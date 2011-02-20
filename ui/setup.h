#ifndef SKIRMISH_SETUP
#define SKIRMISH_SETUP

#include "ui_state.h"

void setup_render(SDL_Surface *, camera *, PLAYERS *, float);
void setup_update(camera *, PLAYERS *);
void setup_handle_event(SDL_Event, camera *, PLAYERS *);
void setup_prepare();
void setup_cleanup();

extern ui_state setup_state;

#endif
