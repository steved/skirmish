#ifndef SKIRMISH_LOADING
#define SKIRMISH_LOADING

#include "ui_state.h"

void loading_render(SDL_Surface *, camera *, player **, int, float);
void loading_update(player **, int, camera *);
void loading_handle_event(SDL_Event, camera *);
void loading_prepare();
void loading_cleanup();
// special loading state
extern ui_state loading_state;

#endif
