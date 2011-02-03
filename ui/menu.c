#include "menu.h"

ui_state menu_state = { &menu_render, &menu_update, &menu_handle_event };

void menu_render(SDL_Surface *buffer, camera *camera, player **players, int player_len) {
}
void menu_update(player **players, int player_len, float interpolation) {
}
void menu_handle_event(SDL_Event event, camera *camera) {
}
