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
        handle_mousedown(event.button, camera);
        break;
      case SDL_QUIT:
        exit(0);
    }
  }

  int mouse_x,mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  int x = 0, y = 0;
  if(mouse_x > WIDTH - mouse_camera_epsilon) 
    x = mouse_camera_delta * ZOOM_LEVEL;
  else if(mouse_x < mouse_camera_epsilon)
    x = -mouse_camera_delta * ZOOM_LEVEL;

  if(mouse_y > HEIGHT - mouse_camera_epsilon)
    y = mouse_camera_delta * ZOOM_LEVEL;
  else if(mouse_y < mouse_camera_epsilon)
    y = -mouse_camera_delta * ZOOM_LEVEL;

  if(x != 0 && y != 0)
    move_camera(camera, x, y);
}

void handle_mousemove(SDL_MouseMotionEvent motion) {
}

void handle_mousedown(SDL_MouseButtonEvent button, camera *camera) {
  printf("Mouse button %d pressed at (%d,%d)\n",
      button.button, button.x, button.y);
  switch(event.button.button) {
    case 4:
      zoom_in(camera);
      break;
    case 5:
      zoom_out(camera);
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
      zoom_in(camera);
      break;
    case SDLK_PAGEDOWN:
      zoom_out(camera);
      break;
    case SDLK_ESCAPE:
      exit(0);
  }
}
