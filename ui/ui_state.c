#include "ui/ui_state.h"
#include "ui/loading.h"

#include "SDL_thread.h"
#include <assert.h>

int prepare_state(void *);
ui_state *current_state = NULL;

void change_state(ui_state *next_state) {
  if(current_state) {
    current_state->cleanup();
  }

  // prepare and set the current state to the loading state 
  current_state = &loading_state;
  loading_state.prepare();

  // spawn a thread to prepare the next state
  // and then set the current_state to it
  prepare_state_thread = SDL_CreateThread(prepare_state, next_state);
}

int prepare_state(void *data) {
  // prepare and set the current state to the next state
  ui_state *state_to_prep = (ui_state *) data;
  state_to_prep->prepare();

  // cleanup the loading state (before it gets switched)
  // and switch to the new state
  SDL_mutexP(current_state_mutex);
  current_state->cleanup();
  current_state = state_to_prep;
  SDL_mutexV(current_state_mutex);

  return 0;
}
