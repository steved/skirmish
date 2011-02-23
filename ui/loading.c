#include "../display.h"
#include "loading.h"

#include "../util/text.h"

// this is a special state that is only set between states
// after a call to change_state, prepare() is called, and
// right after the next state's prepare() is called, cleanup() is called

ui_state loading_state = { &loading_render, &loading_update, &loading_handle_event, &loading_prepare, &loading_cleanup };

SDL_Surface *loading;
SDL_Rect loading_rect;

void loading_render(SDL_Surface *buffer, camera *camera, PLAYERS *players, float interpolation) {

  SDL_BlitSurface(loading, NULL, buffer, &loading_rect);
}

void loading_update(camera *camera, PLAYERS *players) {}
void loading_handle_event(SDL_Event event, camera *camera, PLAYERS *players) {}

void loading_prepare() {
  loading = draw_text(LOADING);

  int w, h;
  TTF_SizeUTF8(font, LOADING, &w, &h);
  loading_rect.x = (WIDTH / 2) - (w / 2);
  loading_rect.y = (HEIGHT / 2) - (h / 2);
  loading_rect.w = w;
  loading_rect.h = h;

  SDL_WM_GrabInput(SDL_GRAB_OFF);
}

void loading_cleanup() {
  SDL_WM_GrabInput(SDL_GRAB_ON);
  SDL_FreeSurface(loading);
}
