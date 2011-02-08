#include "display.h"
#include "events.h"
#include "ui/loading.h"

#include "SDL_events.h"

SDL_Event event;
extern bool game_running;

static void handle_keypress(int, camera *);

void poll_for_events(camera *camera, player **players, int player_len, ui_state *current_state) {
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_KEYDOWN:
        handle_keypress(event.key.keysym.sym, camera);
        break;
      case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == 1 && paused)
          toggle_pause();

        // have to move this out of here and into game.c somehow
        check_for_unit_at(camera, players, player_len, event.button);
        break;
      case SDL_QUIT:
        game_running = false;
    }
    current_state->handle_event(event, camera);
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
