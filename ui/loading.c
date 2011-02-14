#include "../display.h"
#include "loading.h"
#include "../text.h"

// this is a special state that is only set between states
// after a call to change_state, prepare() is called, and
// right after the next state's prepare() is called, cleanup() is called

ui_state loading_state = { &loading_render, &loading_update, &loading_handle_event, &loading_prepare, &loading_cleanup };

void loading_render(SDL_Surface *buffer, camera *camera, PLAYERS *players, float interpolation) {
  int w, h;
  TTF_SizeUTF8(font, "LOADING", &w, &h);
  SDL_Surface *loading_surf = draw_text("LOADING");
  SDL_Rect loading_rect = { (WIDTH / 2) - (w / 2), (HEIGHT / 2) - (h / 2), w, h };
  SDL_BlitSurface(loading_surf, NULL, buffer, &loading_rect);
  SDL_FreeSurface(loading_surf);
}

void loading_update(camera *camera, PLAYERS *players) {}
void loading_handle_event(SDL_Event event, camera *camera, PLAYERS *players) {}

void loading_prepare() {
  SDL_WM_GrabInput(SDL_GRAB_OFF);
}

void loading_cleanup() {
  SDL_WM_GrabInput(SDL_GRAB_ON);
}
