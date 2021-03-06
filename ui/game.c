#include "ui/game.h"
#include "ui/paused.h"

#include "camera.h"
#include "display.h"
#include "selected_actions.h"
#include "units.h"

#include "units/states/unit_state.h"

#include "util/astar.h"
#include "util/text.h"

#include "SDL_gfxPrimitives.h"
#include "SDL_rotozoom.h"

static void update_camera_position(camera *);
static void handle_mousedown(SDL_MouseButtonEvent, camera *, PLAYERS *);
static void handle_keypress(int, camera *);

extern void toggle_pause();
extern bool paused;

ui_state game_state = { &game_render, &game_update, &game_handle_event, &game_prepare, &game_cleanup };

int arrows_camera_delta = 10;
int mouse_camera_delta = 2;
int mouse_camera_epsilon = 10;
int mouse_x = 0, mouse_y = 0;

void game_render(SDL_Surface *buffer, camera *camera, PLAYERS *players, float interpolation) {
  int w, h;

  SDL_Rect terrain_rect = {gsl_vector_get(camera->vector, 0), gsl_vector_get(camera->vector, 1), WIDTH * ZOOM_LEVEL, HEIGHT * ZOOM_LEVEL}; 
  SDL_BlitSurface(background, &terrain_rect, buffer, NULL);

  player *pl;
  division *div;
  unit *un;

  for(int i = 0; i < players->num; i++) {
    pl = players->players[i];
    for(int j = 0; j < pl->num_divisions; j++) {
      div = pl->divisions[j];
      for(int k = 0; k < div->size; k++) {
        un = div->units[k];
        display_unit(buffer, camera, un, pl->color, interpolation);
        /* displays the closest node
        ai_node *closest = find_closest_node(un->vector);
        if(closest == NULL) {
          printf("couldn't find a node for (%f, %f)\n",
              gsl_vector_get(un->vector, 0),
              gsl_vector_get(un->vector, 1));
        } else {
          //printf("closest to (%f, %f) -> (%d, %d)\n", 
          //  gsl_vector_get(un->vector, 0),
          //  gsl_vector_get(un->vector, 1),
          //  closest->x, closest->y);
          gsl_vector *v = calculate_display_position(closest->x, closest->y, camera);
          filledCircleRGBA(buffer, gsl_vector_get(v, 0), gsl_vector_get(v, 1), 3, 0, 0xff, 0, 0xff);
          gsl_vector_free(v);
        }*/
      }
    }
  }

  // display the title in the upper left 
  SDL_BlitSurface(title, NULL, buffer, NULL);

  // display the zoom level in the bottom left
  char zoom[7];
  snprintf(zoom, 7, "Zoom %i", ZOOM_LEVEL);
  TTF_SizeUTF8(font, zoom, &w, &h);
  SDL_Surface *zoom_surf = draw_text(zoom);
  SDL_Rect zo = { 0, HEIGHT - h, w, h };
  SDL_BlitSurface(zoom_surf, NULL, buffer, &zo);
  SDL_FreeSurface(zoom_surf);

  if(paused) {
    paused_state.render(buffer, camera, players, interpolation);
  } else {
    update_camera_position(camera);
  }

}

void game_update(camera *camera, PLAYERS *players) {
  if(paused) {
    paused_state.update(camera, players);
    return;
  }

  for(int i = 0; i < players->num; i++) {
    player *player = players->players[i];
    for(int j = 0; j < player->num_divisions; j++) {
      division *div = player->divisions[j];
      for(int k = 0; k < div->size; k++) {
        update_unit(div->units[k], camera, players);
      }
    }
  }
}

void game_handle_event(SDL_Event event, camera *camera, PLAYERS *players) {
  if(paused) {
    paused_state.handle_event(event, camera, players);
    return;
  }

  switch(event.type) {
    case SDL_MOUSEBUTTONDOWN:
      handle_mousedown(event.button, camera, players);
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
  printf("generating nav_mesh\n");
  walk_terrain();
#ifdef NAV_DEBUG
  draw_nav_mesh(full_terrain, false, true); 
  update_background();
#endif
}

void game_cleanup() {
  SDL_FreeSurface(title);
  if(background != full_terrain)
    SDL_FreeSurface(background);
  SDL_FreeSurface(full_terrain);
  free_nav_mesh();

  paused_state.cleanup();
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

static void handle_mousedown(SDL_MouseButtonEvent button_event, camera *camera, PLAYERS *players) {
  bool modifier = (SDL_GetModState() & KMOD_CTRL) > 0;
  gsl_vector *dest;

  int rel_map_size = MAP_SIZE / ZOOM_LEVEL;
  int start_x = (WIDTH - rel_map_size) / 2;
  int end_x = WIDTH - start_x;
  int start_y = (HEIGHT - rel_map_size) / 2;
  int end_y = HEIGHT - start_y;

  if(button_event.x > end_x || button_event.x < start_x ||
      button_event.y > end_y || button_event.y < start_y)
    return;

  switch(button_event.button) {
    case 1:
      if(paused)
        toggle_pause();
      select_units_at(modifier, button_event.x, button_event.y, camera, players);
      break;
    case 3:
      dest = calculate_map_position(button_event.x, button_event.y, camera);
      unit *un = check_for_unit_near(dest, players, NULL, false, true);
      if(un != NULL) {
        selected_units_attack(un);
      } else {
        move_selected_units_to(dest, players);
      }
      gsl_vector_free(dest);
      break;
    case 4:
      zoom_in(camera);
      update_background();
      break;
    case 5:
      zoom_out(camera);
      update_background();
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
      update_background();
      break;
    case SDLK_PAGEDOWN:
      zoom_out(camera);
      update_background();
      break;
    case SDLK_p:
      toggle_pause();
  }
}

void update_background() {
  // if the zoom level changes, free the
  // current background surface if isn't the base and then re-shrink the surface
  if(background != full_terrain)
    SDL_FreeSurface(background);

  if(ZOOM_LEVEL == 1)
    background = full_terrain;
  else
    background = shrinkSurface(full_terrain, ZOOM_LEVEL, ZOOM_LEVEL);
}
