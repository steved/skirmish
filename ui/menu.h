#ifndef SKIRMISH_MENU
#define SKIRMISH_MENU

#include "ui_state.h"

void menu_render(SDL_Surface *, camera *, player **, int, float);
void menu_update(player **, int);
void menu_handle_event(SDL_Event, camera *);
void menu_cleanup();
// menu state
extern ui_state menu_state;

#endif
