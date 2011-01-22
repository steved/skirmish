#include "events.h"

void poll_for_events(camera *camera) {
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_KEYDOWN:
        handle_keypress(event.key.keysym.sym, camera);
        break;
      case SDL_MOUSEMOTION:
        handle_mousemove(event.motion);
        break;
      case SDL_MOUSEBUTTONDOWN:
        handle_mousedown(event.button);
        break;
      case SDL_QUIT:
        exit(0);
    }
  }

  int mouse_x,mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  int x = 0, y = 0;
  int eps = 10;
  int move = 1;
  if(mouse_x > WIDTH - eps) 
    x = move;
  else if(mouse_x < eps)
    x = -move;

  if(mouse_y > HEIGHT - eps)
    y = move;
  else if(mouse_y < eps)
    y = -move;

  move_camera(camera, x, y);
}

void handle_mousemove(SDL_MouseMotionEvent motion) {
  printf("Mouse moved by %d,%d to (%d,%d)\n", 
      motion.xrel, motion.yrel, motion.x, motion.y);
}

void handle_mousedown(SDL_MouseButtonEvent button) {
  printf("Mouse button %d pressed at (%d,%d)\n",
      button.button, button.x, button.y);
  switch(event.button.button) {
    case 4:
      zoom_in();
      break;
    case 5:
      zoom_out();
  }
}

void handle_keypress(int key, camera *camera) {
  printf("Keypress: %d\n", key);
  switch(key) {
    case SDLK_LEFT:
      move_camera(camera, -15, 0);
      break;
    case SDLK_RIGHT:
      move_camera(camera, 15, 0);
      break;
    case SDLK_UP:
      move_camera(camera, 0, -15);
      break;
    case SDLK_DOWN:
      move_camera(camera, 0, 15);
      break;
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
