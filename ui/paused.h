#ifndef SKIRMISH_PAUSED
#define SKIRMISH_PAUSED

#include "ui_state.h"

void paused_render(SDL_Surface *, camera *, PLAYERS *, float);
void paused_update(camera *, PLAYERS *);
void paused_handle_event(SDL_Event, camera *, PLAYERS *);
void paused_prepare();
void paused_cleanup();
extern ui_state paused_state;

#endif
