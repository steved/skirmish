#ifndef SKIRMISH_LOADING
#define SKIRMISH_LOADING

#include "ui_state.h"

void loading_render(SDL_Surface *, camera *, PLAYERS *, float);
void loading_update(camera *, PLAYERS *);
void loading_handle_event(SDL_Event, camera *, PLAYERS *);
void loading_prepare();
void loading_cleanup();
// special loading state
extern ui_state loading_state;

#endif
