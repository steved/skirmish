#include "../display.h"
#include "game.h"
#include "setup.h"
#include "../terrain.h"
#include "../units.h"

#include "SDL_rotozoom.h"

#include <assert.h>

static void setup_players(PLAYERS *);

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

void setup_update(camera *camera, PLAYERS *players) {
  if(!players->setup) {
    setup_players(players);
    players->setup = true;
  }

  player *pl;
  division *div;
  unit *u;
  for(int i = 0; i < players->num; i++) {
    pl = players->players[i];
    for(int j = 0; j < pl->num_divisions; j++) {
      div = pl->divisions[j];
      for(int k = 0; k < div->size; k++) {
        u = div->units[k];
        if(u->state.current == moving) {
          place_at_vector(u, u->state.subject.vector);
          u->state.current = waiting;
        }
      }
    }
  }
}

void setup_prepare() {
  // background hasn't been generated; do it
  if(full_terrain == NULL) {
    generate_fractal_terrain();
    full_terrain = print_terrain();
    background = shrinkSurface(full_terrain, ZOOM_LEVEL, ZOOM_LEVEL);
  }

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
        break;
      default:
        break;
    }
  }
}

void setup_cleanup() {}

void setup_players(PLAYERS *players) {
  player *player;
  division *div;
  unit *u;

  int quarter_map_size = HALF_MAP_SIZE / 2;
  int total_area_per_player = HALF_MAP_SIZE * quarter_map_size;

  int area;

  int ai_positions[][2] = {
    //{ HALF_MAP_SIZE, quarter_map_size }, { quarter_map_size, HALF_MAP_SIZE }, { HALF_MAP_SIZE + quarter_map_size, HALF_MAP_SIZE } 
    { quarter_map_size, quarter_map_size }, { quarter_map_size, quarter_map_size }, { HALF_MAP_SIZE + quarter_map_size, quarter_map_size } 
  };
  // top = 0; left = 1; right = 2
  int pos_index = -1;
  int x, y, rad;

  for(int i = 0; i < players->num; i++) {
    player = players->players[i];

    if(player->human) {
      x = quarter_map_size;
      y = HALF_MAP_SIZE + quarter_map_size;
    } else {
      x = ai_positions[++pos_index][0];
      y = ai_positions[pos_index][1];
    }
    area = 0;

    for(int j = 0; j < player->num_divisions; j++) {
      div = player->divisions[j];
      for(int k = 0; k < div->size; k++) {
        u = div->units[k];
        place(u, x, y);
        rad = unit_radius[u->type] * 2;
        area += rad * rad;

        if(pos_index == 0 || player->human) {
          x += rad; 
          if(x > HALF_MAP_SIZE + quarter_map_size) {
            x = ai_positions[pos_index][0];
            y += (player->human ? 1 : -1) * rad;
          }
        } else {
          y += rad;
          if(y > HALF_MAP_SIZE + quarter_map_size) {
            y = ai_positions[pos_index][1];
            x += (pos_index == 1 ? -1 : 1) * rad;
          }
        }
      }
    }

    assert(area <= total_area_per_player);
  }
  // place the armies
}
