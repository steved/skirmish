#include "ui_state.h"

ui_state *current_state = NULL;

void change_state(ui_state *next_state) {
  if(current_state)
    current_state->cleanup();
  current_state = next_state;
}
