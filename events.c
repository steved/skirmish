#include "display.h"
#include "events.h"

#include "SDL_events.h"

SDL_Event event;
int arrows_camera_delta = 10;
int mouse_camera_delta = 2;
int mouse_camera_epsilon = 10;
int mouse_x = 0, mouse_y = 0;

void handle_keypress(int, camera *);
static void handle_mousedown(SDL_MouseButtonEvent, camera *);
static void handle_mousemove(SDL_MouseMotionEvent);

void poll_for_events(camera *camera, player **players, int player_len) {
  while(SDL_PollEvent(&event)) {
    if(paused) {
      if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_p)
          toggle_pause();
        else if(event.key.keysym.sym == SDLK_ESCAPE)
          exit(0);
      }
    } else {
      switch(event.type) {
        case SDL_KEYDOWN:
          handle_keypress(event.key.keysym.sym, camera);
          break;
        case SDL_MOUSEMOTION:
          handle_mousemove(event.motion);
          break;
        case SDL_MOUSEBUTTONDOWN:
          handle_mousedown(event.button, camera);
          check_for_unit_at(camera, players, player_len, event.button);
          break;
        case SDL_QUIT:
          exit(0);
      }
    }
  }

  // don't attempt to move the camera if paused
  if(paused)
    return;

  // code to move the camera if the mouse
  // is within mouse_camera_epsilon of the edges
  int x = 0, y = 0;
  if(mouse_x > WIDTH - mouse_camera_epsilon) 
    x = mouse_camera_delta * ZOOM_LEVEL;
  else if(mouse_x < mouse_camera_epsilon)
    x = -mouse_camera_delta * ZOOM_LEVEL;

  if(mouse_y > HEIGHT - mouse_camera_epsilon)
    y = mouse_camera_delta * ZOOM_LEVEL;
  else if(mouse_y < mouse_camera_epsilon)
    y = -mouse_camera_delta * ZOOM_LEVEL;

  if(x != 0 || y != 0)
    move_camera(camera, x, y);
}

static void handle_mousemove(SDL_MouseMotionEvent motion) {
  mouse_x = motion.x;
  mouse_y = motion.y;
}

static void handle_mousedown(SDL_MouseButtonEvent button, camera *camera) {
  printf("Mouse button %d pressed at (%d,%d)\n",
      button.button, button.x, button.y);
  switch(event.button.button) {
    case 1:
      if(paused)
        toggle_pause();
      break;
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
      move_camera(camera, -arrows_camera_delta * ZOOM_LEVEL, 0);
      break;
    case SDLK_RIGHT:
      move_camera(camera, arrows_camera_delta * ZOOM_LEVEL, 0);
      break;
    case SDLK_UP:
      move_camera(camera, 0, -arrows_camera_delta * ZOOM_LEVEL);
      break;
    case SDLK_DOWN:
      move_camera(camera, 0, arrows_camera_delta * ZOOM_LEVEL);
      break;
    case SDLK_PAGEUP:
      zoom_in(camera);
      break;
    case SDLK_PAGEDOWN:
      zoom_out(camera);
      break;
    case SDLK_p:
      toggle_pause();
      break;
    case SDLK_ESCAPE:
      exit(0);
  }
}
