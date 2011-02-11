#ifndef SKIRMISH_GAME
#define SKIRMISH_GAME

#include "ui_state.h"

void game_render(SDL_Surface *, camera *, player **, int, float);
void game_update(player **, int, camera *);
void game_handle_event(SDL_Event, camera *);
void game_prepare();
void game_cleanup();
extern ui_state game_state;
extern bool paused;

#endif
