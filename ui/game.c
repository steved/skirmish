#include "../camera.h"
#include "../display.h"
#include "game.h"
#include "../text.h"
#include "../terrain.h"
#include "../units.h"

#include "SDL_rotozoom.h"

static void update_camera_position(camera *);
static void handle_mousedown(SDL_MouseButtonEvent, camera *);
static void handle_keypress(int, camera *);

ui_state game_state = { &game_render, &game_update, &game_handle_event, &game_prepare, &game_cleanup };

int prev_zoom_level = -1;
SDL_Surface *full_terrain = NULL;
SDL_Surface *background = NULL;

int arrows_camera_delta = 10;
int mouse_camera_delta = 2;
int mouse_camera_epsilon = 10;
int mouse_x = 0, mouse_y = 0;

void game_render(SDL_Surface *buffer, camera *camera, player **players, int player_len, float interpolation) {
  int w, h;

  SDL_Rect terrain_rect = {gsl_vector_get(camera->vector, 0), gsl_vector_get(camera->vector, 1), WIDTH * ZOOM_LEVEL, HEIGHT * ZOOM_LEVEL}; 
  // if the zoom level changes, free the
  // current background surface if isn't the base and then re-shrink the surface
  if(prev_zoom_level != ZOOM_LEVEL) {
    if(background != full_terrain)
      SDL_FreeSurface(background);
    background = shrinkSurface(full_terrain, ZOOM_LEVEL, ZOOM_LEVEL);
  }  
  SDL_BlitSurface(background, &terrain_rect, buffer, NULL);

  for(int i = 0; i < player_len; i++) {
    player *player = players[i];
    for(int j = 0; j < player->num_units; j++) {
      display_unit(buffer, camera, player->units[j], player->color, interpolation);
    }
  }

  // display the title in the upper left 
  SDL_Surface *title = draw_text("Skirmish");
  SDL_BlitSurface(title, NULL, buffer, NULL);
  SDL_FreeSurface(title);

  // display the zoom level in the bottom left
  char zoom[7];
  snprintf(zoom, 7, "Zoom %i", ZOOM_LEVEL);
  TTF_SizeUTF8(font, zoom, &w, &h);
  SDL_Surface *zoom_surf = draw_text(zoom);
  SDL_Rect zo = { 0, HEIGHT - h, w, h };
  SDL_BlitSurface(zoom_surf, NULL, buffer, &zo);
  SDL_FreeSurface(zoom_surf);

  if(paused) {
    TTF_SizeUTF8(font, "PAUSED", &w, &h);
    SDL_Surface *paused_surf = draw_text("PAUSED");
    SDL_Rect paused_rect = { (WIDTH / 2) - (w / 2), (HEIGHT / 2) - (h / 2), w, h };

    SDL_Surface *overlay = SDL_CreateRGBSurface(SDL_SRCALPHA, WIDTH, HEIGHT, buffer->format->BitsPerPixel, 0, 0, 0, 0);
    SDL_FillRect(overlay, NULL, SDL_MapRGB(buffer->format, 0, 0, 0));
    SDL_SetAlpha(overlay, SDL_SRCALPHA, 128);

    SDL_BlitSurface(overlay, NULL, buffer, NULL);
    SDL_BlitSurface(paused_surf, NULL, buffer, &paused_rect);
    SDL_FreeSurface(overlay);
    SDL_FreeSurface(paused_surf);
  } else {
    update_camera_position(camera);
  }

  prev_zoom_level = ZOOM_LEVEL;
}

void game_update(player **players, int player_len, camera *camera) {
  for(int i = 0; i < player_len; i++) {
    for(int j = 0; j < players[i]->num_units; j++) {
      unit *u = players[i]->units[j];
      state_functions[u->state.current](u);
    }
  }
}

void game_handle_event(SDL_Event event, camera *camera) {
  switch(event.type) {
    case SDL_MOUSEBUTTONDOWN:
      handle_mousedown(event.button, camera);
      break;
    case SDL_MOUSEMOTION:
      mouse_x = event.motion.x;
      mouse_y = event.motion.y;
      break;
    case SDL_KEYDOWN:
      handle_keypress(event.key.keysym.sym, camera);
  }
}

void game_prepare() {
  // background hasn't been generated; do it
  if(full_terrain == NULL) {
    generate_fractal_terrain();
    full_terrain = print_terrain();
    background = shrinkSurface(full_terrain, ZOOM_LEVEL, ZOOM_LEVEL);
  }
}

void game_cleanup() {
  SDL_FreeSurface(background);
  SDL_FreeSurface(full_terrain);
}

static void update_camera_position(camera *camera) {
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

static void handle_mousedown(SDL_MouseButtonEvent button_event, camera *camera) {
  switch(button_event.button) {
    case 4:
      zoom_in(camera);
      break;
    case 5:
      zoom_out(camera);
  }
}

static void handle_keypress(int key, camera *camera) {
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
  }
}
