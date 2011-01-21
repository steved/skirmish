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
        handle_mousedown(event.button.button, event.button.x, event.button.y);
        break;
      case SDL_QUIT:
        exit(0);
    }
  }
}

void handle_mousedown(int button, int x, int y) {
  printf("Mouse button %d pressed at (%d,%d)\n",
      event.button.button, event.button.x, event.button.y);
  switch(event.button.button) {
    case 4:
      zoom_in();
      break;
    case 5:
      zoom_out();
  }
}

void handle_keypress(int key) {
  printf("Keypress: %d\n", key);
  switch(key) {
    case SDLK_PAGEUP:
      zoom_in();
      break;
    case SDLK_PAGEDOWN:
      zoom_out();
      break;
    case SDLK_ESCAPE:
      exit(0);
  }
}
