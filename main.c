#include "attributes.h"
#include "camera.h"
#include "display.h"
#include "events.h"
#include "main.h"
#include "player.h"
#include "romans.h"
#include "selected.h"
#include "text.h"
#include "units.h"

#include "ui/game.h"
#include "ui/menu.h"
#include "ui/ui_state.h"

#include <assert.h>
#include <time.h>

#define TICKS_PER_SECOND 25
#define MAX_FRAME_SKIP 5
const float SKIP_TICKS = 1000 / TICKS_PER_SECOND;
player *human;
bool game_running = true;

int main(int argc, char *argv[]) {
  current_state_mutex = SDL_CreateMutex();
  srand(time(NULL));

  player *players[2];

  printf("creating camera\n");
  camera *camera = create_camera();

  printf("creating ai player\n");
  players[0] = create_ai_player(1);

  division *div = (division *) malloc(sizeof(division));
  assert(div != NULL);

  div->units = (unit **) malloc(sizeof(unit *) * 2); 
  div->size = 2;
  assert(div->units != NULL);

  unit *un;
  for(int i = 0; i < 2; i++) {
    un = create_legionary_unit();

    /*
    if(i == 0) {
      un->state.current = moving;
      gsl_vector *v = gsl_vector_alloc(2);
      gsl_vector_set(v, 0, 600);
      gsl_vector_set(v, 1, 600);
      un->state.subject.vector = v;
    }*/

    un->division_idx = 0;
    place(un, 600, 600 + (i * 50));
    div->units[i] = un;
  }
  players[0]->divisions[0] = div;

  player *human = create_human_player("Steven Davidovitz", 1);

  div = (division *) malloc(sizeof(division));
  assert(div != NULL);

  div->units = (unit **) malloc(sizeof(unit *) * 2); 
  div->size = 2;
  assert(div->units != NULL);

  for(int i = 0; i < 2; i++) {
    un = create_legionary_unit();
    un->division_idx = 0;
    place(un, 250 + (i * 50), 250);
    div->units[i] = un;
  }
  human->divisions[0] = div;
  players[1] = human;

  if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  } 
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  SDL_WM_GrabInput(SDL_GRAB_ON);
  atexit(SDL_Quit);

  const SDL_VideoInfo *info = SDL_GetVideoInfo();
  if(!info) {
    fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
    exit(1);
  }
  int bpp = info->vfmt->BitsPerPixel;

  screen = SDL_SetVideoMode(WIDTH, HEIGHT, bpp, SDL_SWSURFACE);
  if(screen == 0) {
    fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
    exit(1);
  }

  if(init_ttf() == -1) {
    fprintf(stderr, "Unable to set TTF.\n");
    exit(1);
  }

  change_state(&menu_state);

  uint32_t next_game_tick = SDL_GetTicks();

  int loops;
  float interpolation;

  while(game_running) {
    SDL_mutexP(current_state_mutex);
    assert(current_state != NULL);

    poll_for_events(camera, players, 2, current_state);
    loops = 0;

    while(SDL_GetTicks() > next_game_tick && loops < MAX_FRAME_SKIP) {
      current_state->update(players, 2, camera);
      next_game_tick += SKIP_TICKS;
      loops++;
    }

    SDL_Surface *buffer = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, bpp, 0, 0, 0, 0xff);
    interpolation = (SDL_GetTicks() + SKIP_TICKS - next_game_tick) / SKIP_TICKS;
    current_state->render(buffer, camera, players, 2, interpolation);

    SDL_mutexV(current_state_mutex);
    SDL_BlitSurface(buffer, NULL, screen, NULL);
    SDL_Flip(screen);
    SDL_FreeSurface(buffer);
  }

  current_state->cleanup();

  for(int i = 0; i < 2; i++)
    remove_player(players[i]);
  remove_camera(camera);

  SDL_FreeSurface(screen);
  return 0;
}

void toggle_pause() {
  paused = !paused;
  if(paused)
    SDL_WM_GrabInput(SDL_GRAB_OFF);
  else
    SDL_WM_GrabInput(SDL_GRAB_ON);
}
