#include "loading.h"
#include "ui_state.h"

#include "SDL_thread.h"

int prepare_state(void *);
ui_state *current_state = NULL;

void change_state(ui_state *next_state) {
  if(current_state)
    current_state->cleanup();

  // prepare and set the current state to the loading state 
  loading_state.prepare();
  current_state = &loading_state;

  // spawn a thread to prepare the next state
  // and then set the current_state to it
  SDL_CreateThread(prepare_state, next_state);
}

int prepare_state(void *data) {
  // prepare and set the current state to the next state
  ui_state *state_to_prep = (ui_state *) data;
  state_to_prep->prepare();
  current_state = state_to_prep;

  // cleanup the loading state before it gets switched
  loading_state.cleanup();
  return 0;
}
