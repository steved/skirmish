#ifndef SKIRMISH_MENU
#define SKIRMISH_MENU

#include "ui_state.h"

void menu_render(SDL_Surface *, camera *, PLAYERS *, float);
void menu_update(camera *, PLAYERS *);
void menu_handle_event(SDL_Event, camera *, PLAYERS *);
void menu_prepare();
void menu_cleanup();
// menu state
extern ui_state menu_state;

#endif
