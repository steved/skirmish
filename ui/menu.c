#include "ui/menu.h"
#include "ui/setup.h"

ui_state menu_state = { &menu_render, &menu_update, &menu_handle_event, &menu_prepare, &menu_cleanup };

void menu_render(SDL_Surface *buffer, camera *camera, PLAYERS *players, float interpolation) {
}

void menu_update(camera *camera, PLAYERS *players) {
  change_state(&setup_state);
}

void menu_handle_event(SDL_Event event, camera *camera, PLAYERS *players) {}
void menu_prepare() {}
void menu_cleanup() {}
