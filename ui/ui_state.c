#include "loading.h"
#include "ui_state.h"

#include "SDL_thread.h"

int prepare_state(void *);
ui_state *current_state = NULL;

void change_state(ui_state *next_state) {
  if(current_state)
    current_state->cleanup();
  SDL_CreateThread(prepare_state, next_state);
  current_state = &loading_state;
}

int prepare_state(void *data) {
  ui_state *state_to_prep = (ui_state *) data;
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  state_to_prep->prepare();
  SDL_WM_GrabInput(SDL_GRAB_ON);
  current_state = state_to_prep;
  return 0;
}
