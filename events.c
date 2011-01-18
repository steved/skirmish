#include "events.h"

void poll_for_events() {
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_KEYDOWN:
        handle_keypress(event.key.keysym.sym);
        break;
      case SDL_MOUSEMOTION:
        printf("Mouse moved by %d,%d to (%d,%d)\n", 
            event.motion.xrel, event.motion.yrel,
            event.motion.x, event.motion.y);
        break;
      case SDL_MOUSEBUTTONDOWN:
        printf("Mouse button %d pressed at (%d,%d)\n",
            event.button.button, event.button.x, event.button.y);
        break;
      case SDL_QUIT:
        exit(0);
    }
  }
}

void handle_keypress(int key) {
  printf("Keypress: %d\n", key);
  switch(key) {
    case SDLK_ESCAPE:
      exit(0);
  }
}
