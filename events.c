#include "display.h"
#include "events.h"
#include "ui/loading.h"

#include "SDL_events.h"

SDL_Event event;
extern bool game_running;

static void handle_keypress(int, camera *);

void poll_for_events(camera *camera, PLAYERS *players, ui_state *current_state) {
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_KEYDOWN:
        handle_keypress(event.key.keysym.sym, camera);
        break;
      case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == 1 && paused)
          toggle_pause();

        break;
      case SDL_QUIT:
        game_running = false;
    }
    current_state->handle_event(event, camera, players);
  }
}

static void handle_keypress(int key, camera *camera) {
  printf("Keypress: %d\n", key);
  switch(key) {
    case SDLK_p:
      toggle_pause();
      break;
    case SDLK_ESCAPE:
      game_running = false;
  }
}
