#include "ui/setup.h"
#include "ui/game.h"
#include "ui/paused.h"

#include "collision.h"
#include "display.h"
#include "units.h"

#include "util/nav_mesh.h"
#include "util/terrain.h"
#include "util/text.h"

#include "units/states/unit_state.h"

#include "SDL_rotozoom.h"
#include "SDL_gfxPrimitives.h"

#include <assert.h>

static void regenerate_terrain();
static void setup_players(PLAYERS *);
static void advance_unit_position(int, int *, int *, int);

ui_state setup_state = { &setup_render, &setup_update, &setup_handle_event, &setup_prepare, &setup_cleanup };
SDL_Surface *full_terrain = NULL;
SDL_Surface *background, *title, *instructions;
SDL_Rect instruction_rect;

// must have at least one human player, so can have max MAX_PLAYERS - 1 ai players
static int quarter_map_size, total_area_per_player, ai_positions[MAX_PLAYERS - 1][2];

void setup_render(SDL_Surface *buffer, camera *camera, PLAYERS *players, float interpolation) {
  game_render(buffer, camera, players, interpolation);
  SDL_BlitSurface(instructions, NULL, buffer, &instruction_rect);
}

void setup_update(camera *camera, PLAYERS *players) {
  if(!players->setup) {
    setup_players(players);
    players->setup = true;
  }
}

void setup_prepare() {
  title = draw_text("Skirmish");

  // display the zoom level in the bottom left
  char *instr_text = "SETUP: Press ENTER to leave";
  int w,h;
  TTF_SizeUTF8(font, instr_text, &w, &h);
  instructions = draw_text(instr_text);
  instruction_rect.x = WIDTH - w;
  instruction_rect.y = HEIGHT - h;
  instruction_rect.w = w;
  instruction_rect.h = h;

  // background hasn't been generated; do it
  regenerate_terrain();

  paused_state.prepare();
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
      case SDLK_r:
        players->setup = false;
        change_state(&setup_state);
        break;
      default:
        break;
    }
  }
}

void setup_cleanup() {
  SDL_FreeSurface(instructions);
}

void setup_players(PLAYERS *players) {
  printf("setting up AI players\n");

  player *player;
  division *div;
  unit *u;

  int area;

  quarter_map_size = HALF_MAP_SIZE / 2;
  total_area_per_player = HALF_MAP_SIZE * quarter_map_size;

  ai_positions[0][0] = quarter_map_size;
  ai_positions[0][1] = quarter_map_size;
  
  ai_positions[1][0] = quarter_map_size;
  ai_positions[1][1] = quarter_map_size;
  
  ai_positions[2][0] = HALF_MAP_SIZE + quarter_map_size;
  ai_positions[2][1] = quarter_map_size;

  // top = 0; left = 1; right = 2
  int pos_index = -1;
  int x, y, rad, pos, mult;

  for(int i = 0; i < players->num; i++) {
    player = players->players[i];

    if(player->human) {
      pos = -1;
      x = quarter_map_size;
      y = HALF_MAP_SIZE + quarter_map_size;
      mult = 1;
    } else {
      pos = ++pos_index;
      x = ai_positions[pos][0];
      y = ai_positions[pos][1];
      mult = (pos == 2) ? 1 : -1;
    }
    area = 0;

    int div_x = x, div_y = y;
    int unit_x = x, unit_y = y;
    int max_depth = 0;

    for(int j = 0; j < player->num_divisions; j++) {
      div = player->divisions[j];

      if(pos == 0 || player->human) {
        // check to see if div_x is > the allotted width
        // TODO maybe set allotted width in player
        if(div_x > x + quarter_map_size) {
          div_x = x;
          div_y = max_depth + player->column_padding;
          max_depth = 0;
        }
        unit_y = div_y;
      } else {
        // check to see if div_y is > the allotted height
        if(div_y > y + quarter_map_size) {
          div_y = y;
          div_x = max_depth + player->column_padding; 
          max_depth = 0;
        }
        unit_x = div_x;
      }

      for(int k = 0; k < div->size; k++) {
        if(k % div->structure.num_per_row == 0) {
          if(pos == 0 || player->human) {
            unit_x = div_x;
            unit_y += div->structure.row_padding * mult;
          } else {
            unit_x += div->structure.row_padding * mult;
            unit_y = div_y;
          }
        }

        u = div->units[k];
        rad = u->collision_radius * 2;
        place(u, unit_x, unit_y);
        area += rad * rad;

        while(!allowed_on_terrain(u->position)) {
          advance_unit_position(pos, &x, &y, rad);
          place(u, unit_x, unit_y);
          area += rad * rad;
        }

        advance_unit_position(pos, &unit_x, &unit_y, rad);
      }

      if(pos == 0 || player->human) {
        if(unit_y > max_depth)
          max_depth = unit_y;
        div_x = unit_x + player->column_padding; 
      } else {
        if(unit_x > max_depth)
          max_depth = unit_x;
        div_y = unit_y + player->column_padding;
      }
    }

    assert(area <= total_area_per_player);
  }
}

static void advance_unit_position(int pos_index, int *x, int *y, int rad) {
  if(pos_index <= 0) {
    *x += rad; 
    if(*x > HALF_MAP_SIZE + quarter_map_size) {
      *x = ai_positions[pos_index][0];
      *y -= rad;
    }
  } else {
    *y += rad;
    if(*y > HALF_MAP_SIZE + quarter_map_size) {
      *y = ai_positions[pos_index][1];
      *x += (pos_index == 1 ? -1 : 1) * rad;
    }
  }
}

static void regenerate_terrain() {
  if(full_terrain != NULL) {
    if(background != full_terrain)
      SDL_FreeSurface(background);
    SDL_FreeSurface(full_terrain);

    full_terrain = NULL;
    background = NULL;
  }

  printf("generating terrain\n");
  generate_fractal_terrain();
  full_terrain = print_terrain();
  update_background();
}
