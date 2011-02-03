#include "ui_state.h"

void menu_render(SDL_Surface *, camera *, player *, int);
void menu_update(player *, int);
void menu_handle_event(SDL_Event, camera *);
// menu state
ui_state menu = { &menu_render, &menu_update, &menu_handle_event };

void menu_render(SDL_Surface *buffer, camera *camera, player *players, int player_len) {
}
void menu_update(player *players, int player_len) {
}
void menu_handle_event(SDL_Event event, camera *camera) {
}
