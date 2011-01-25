#include "events.h"
int arrows_camera_delta = 10;
int mouse_camera_delta = 1;
int mouse_camera_epsilon = 10;

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
  if(mouse_x > WIDTH - mouse_camera_epsilon) 
    x = mouse_camera_delta;
  else if(mouse_x < mouse_camera_epsilon)
    x = -mouse_camera_delta;

  if(mouse_y > HEIGHT - mouse_camera_epsilon)
    y = mouse_camera_delta;
  else if(mouse_y < mouse_camera_epsilon)
    y = -mouse_camera_delta;

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
      move_camera(camera, -arrows_camera_delta, 0);
      break;
    case SDLK_RIGHT:
      move_camera(camera, arrows_camera_delta, 0);
      break;
    case SDLK_UP:
      move_camera(camera, 0, -arrows_camera_delta);
      break;
    case SDLK_DOWN:
      move_camera(camera, 0, arrows_camera_delta);
      break;
    case SDLK_PAGEUP:
      zoom_in();
      if(MAP_SIZE / ZOOM_LEVEL == WIDTH) {
        set_camera_position(camera, 0, 0);
      }
      break;
    case SDLK_PAGEDOWN:
      zoom_out();
      break;
    case SDLK_ESCAPE:
      exit(0);
  }
}
