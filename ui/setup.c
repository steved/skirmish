#include "../display.h"
#include "game.h"
#include "setup.h"
#include "../terrain.h"

#include "SDL_rotozoom.h"

ui_state setup_state = { &setup_render, &setup_update, &setup_handle_event, &setup_prepare, &setup_cleanup };
SDL_Surface *full_terrain = NULL;
SDL_Surface *background;

//static int prev_zoom_level = -1;

void setup_render(SDL_Surface *buffer, camera *camera, PLAYERS *players, float interpolation) {
  game_render(buffer, camera, players, interpolation);
  /*
  SDL_Rect terrain_rect = {gsl_vector_get(camera->vector, 0), gsl_vector_get(camera->vector, 1), WIDTH * ZOOM_LEVEL, HEIGHT * ZOOM_LEVEL}; 
  // if the zoom level changes, free the
  // current background surface if isn't the base and then re-shrink the surface
  if(prev_zoom_level != ZOOM_LEVEL) {
    if(background != full_terrain)
      SDL_FreeSurface(background);
    background = shrinkSurface(full_terrain, ZOOM_LEVEL, ZOOM_LEVEL);
  }
  prev_zoom_level = ZOOM_LEVEL;
  SDL_BlitSurface(background, &terrain_rect, buffer, NULL);*/
}

void setup_update(camera *camera, PLAYERS *players) {}

void setup_prepare() {
  // background hasn't been generated; do it
  if(full_terrain == NULL) {
    generate_fractal_terrain();
    full_terrain = print_terrain();
    background = shrinkSurface(full_terrain, ZOOM_LEVEL, ZOOM_LEVEL);
  }

  // place the armies
}

void setup_handle_event(SDL_Event event, camera *camera, PLAYERS *players) {
  if(paused)
    return;

  game_handle_event(event, camera, players);
  
  if(event.type == SDL_KEYDOWN) {
    switch(event.key.keysym.sym) {
      case SDLK_RETURN:
        change_state(&game_state);
        break;
      case SDLK_0:
        printf("%s\n", players->players[0]->name);
    }
  }
}

void setup_cleanup() {}
