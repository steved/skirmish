#ifndef SKIRMISH_GAME
#define SKIRMISH_GAME

#include "ui_state.h"

void game_render(SDL_Surface *, camera *, PLAYERS *, float);
void game_update(camera *, PLAYERS *);
void game_handle_event(SDL_Event, camera *, PLAYERS *);
void game_prepare();
void game_cleanup();
void update_background();

extern ui_state game_state;
extern bool paused;
extern SDL_Surface *full_terrain, *background, *title;

#endif
