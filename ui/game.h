#ifndef SKIRMISH_GAME
#define SKIRMISH_GAME

#include "ui_state.h"

void game_render(SDL_Surface *, camera *, player **, int);
void game_update(player **, int, float);
void game_handle_event(SDL_Event, camera *);
extern ui_state game_state;

#endif
