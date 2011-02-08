#include "ui_state.h"

#include "SDL_thread.h"

int prepare_state(void *);
ui_state *current_state = NULL;

void change_state(ui_state *next_state) {
  if(current_state)
    current_state->cleanup();
  SDL_CreateThread(prepare_state, next_state);
  current_state = next_state;
}

int prepare_state(void *data) {
  ui_state *state_to_prep = (ui_state *) data;
  state_to_prep->preparing = true;
  state_to_prep->prepare();
  state_to_prep->preparing = false;
  return 0;
}
