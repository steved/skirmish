#include "game.h"
#include "menu.h"

ui_state menu_state = { &menu_render, &menu_update, &menu_handle_event, &menu_cleanup };

void menu_render(SDL_Surface *buffer, camera *camera, player **players, int player_len, float interpolation) {
}
void menu_update(player **players, int player_len) {
  change_state(&game_state);
}
void menu_handle_event(SDL_Event event, camera *camera) {
}
void menu_cleanup() {
}
